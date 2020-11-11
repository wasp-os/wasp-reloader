/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#ifndef RELOADER_SPI_H__
#define RELOADER_SPI_H__

#include <stdint.h>

void spi_init(void);
void spi_teardown(void);
void spi_read(uint8_t *data, unsigned len);
void spi_write(const uint8_t *data, unsigned len);

#endif /* RELOADER_SPI_H__ */
