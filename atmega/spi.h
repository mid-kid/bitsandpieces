#pragma once

#include <stdint.h>

enum spi_divider {
    SPI_DIV4,
    SPI_DIV16,
    SPI_DIV64,
    SPI_DIV128,

    // Double speed
    SPI_DIV2,
    SPI_DIV8,
    SPI_DIV32,
    SPI_DIV64_2
};

void spi_init(const enum spi_divider divider);
void spi_off(void);
uint8_t spi_transfer(const uint8_t data);
