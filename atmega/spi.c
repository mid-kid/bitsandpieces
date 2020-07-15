#include "spi.h"

#include <stdint.h>
#include <avr/io.h>

#include "utils.h"
#include "pins.h"

void spi_init(const enum spi_divider divider)
{
    DDRB |= pin(PIN_SPI_SS) | pin(PIN_SPI_MOSI) | pin(PIN_SPI_SCK);  // Outputs
    DDRB &= ~pin(PIN_SPI_MISO);  // Inputs
    PORTB |= pin(PIN_SPI_MISO);  // Pull it up

    // Configure SPI as master
    SPCR = _BV(SPE) | _BV(MSTR) | (divider & 3);
    SPSR = (divider & 4) >> 2;
}

void spi_off(void)
{
    SPCR = 0;
    SPSR = 1;
}

uint8_t spi_transfer(const uint8_t data)
{
    SPDR = data;
    __asm__ volatile ("nop");
    while (!(SPSR & _BV(SPIF))) __asm__ volatile ("nop");
    return SPDR;
}
