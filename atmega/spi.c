#include "spi.h"

#include <stdint.h>
#include <avr/io.h>

#include "utils.h"

// NOTE: This module just supports sending through hardware SPI right now.

#define SPI_SS PORTB2
#define SPI_MOSI PORTB3
#define SPI_MISO PORTB4
#define SPI_CLK PORTB5

void spi_init_master(const int divider)
{
    DDRB |= _BV(SPI_SS) | _BV(SPI_MOSI) | _BV(SPI_CLK);  // Outputs
    DDRB &= ~_BV(SPI_MISO);  // Inputs
    PORTB |= _BV(SPI_MISO);  // Pull it up

    // Configure SPI as master
    SPCR = _BV(SPE) | _BV(MSTR) | (divider & 3);
    SPSR = (divider & 4) >> 2;
}

void spi_off(void)
{
    SPCR = 0;
    SPSR = 1;
}

void spi_transfer(const uint8_t data)
{
    SPDR = data;
    __asm__ volatile ("nop");
    while (!(SPSR & _BV(SPIF)));
}
