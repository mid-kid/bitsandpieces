#pragma once

#include <stdint.h>

#define SPI_DIV4 0
#define SPI_DIV16 1
#define SPI_DIV64 2
#define SPI_DIV128 3

// Double speed SPI
#define SPI_DIV2 4
#define SPI_DIV8 5
#define SPI_DIV32 6
#define SPI_DIV64_2 7

void spi_init_master(const int divider);
void spi_off(void);
void spi_transfer(const uint8_t data);
