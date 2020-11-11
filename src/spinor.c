/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#include "spinor.h"

#include <nrf_gpio.h>

#include "board.h"
#include "spi.h"

#ifdef CONFIG_HAVE_SPINOR

const uint8_t PP[] = { 0x02 };
const uint8_t RDSR[] = { 0x05 };
const uint8_t WREN[] = { 0x06 };
const uint8_t SE[] = { 0x20 };
const uint8_t RDP[] = { 0xab };

uint32_t spinor_page_size_get(void)
{
    return 256;
}

uint32_t spinor_sector_size_get(void)
{
    return 4096;
}

void spinor_sector_erase(uint32_t addr)
{
    uint8_t cmd[] = { SE[0], addr >> 16, addr >> 8, addr };
    uint8_t status;

    do {
        nrf_gpio_pin_clear(CONFIG_SPINOR_CS);
        spi_write(WREN, sizeof(WREN));
        nrf_gpio_pin_set(CONFIG_SPINOR_CS);

        nrf_gpio_pin_clear(CONFIG_SPINOR_CS);
	spi_write(RDSR, sizeof(RDSR));
	spi_read(&status, 1);
	nrf_gpio_pin_set(CONFIG_SPINOR_CS);
    } while (0 == (status & 2));

    nrf_gpio_pin_clear(CONFIG_SPINOR_CS);
    spi_write(cmd, sizeof(cmd));
    nrf_gpio_pin_set(CONFIG_SPINOR_CS);

    do {
        nrf_gpio_pin_clear(CONFIG_SPINOR_CS);
	spi_write(RDSR, sizeof(RDSR));
	spi_read(&status, 1);
	nrf_gpio_pin_set(CONFIG_SPINOR_CS);
    } while (status & 1);
}

void spinor_burst_write(uint32_t addr, const uint8_t *buf, uint32_t buflen)
{
    uint8_t cmd[] = { PP[0], addr >> 16, addr >> 8, addr };
    uint8_t status;

    do {
        nrf_gpio_pin_clear(CONFIG_SPINOR_CS);
        spi_write(WREN, sizeof(WREN));
        nrf_gpio_pin_set(CONFIG_SPINOR_CS);

        nrf_gpio_pin_clear(CONFIG_SPINOR_CS);
	spi_write(RDSR, sizeof(RDSR));
	spi_read(&status, 1);
	nrf_gpio_pin_set(CONFIG_SPINOR_CS);
    } while (0 == (status & 2));

    nrf_gpio_pin_clear(CONFIG_SPINOR_CS);
    spi_write(cmd, sizeof(cmd));
    spi_write(buf, buflen);
    nrf_gpio_pin_set(CONFIG_SPINOR_CS);

    do {
        nrf_gpio_pin_clear(CONFIG_SPINOR_CS);
	spi_write(RDSR, sizeof(RDSR));
	spi_read(&status, 1);
	nrf_gpio_pin_set(CONFIG_SPINOR_CS);
    } while (status & 1);
}

void spinor_word_write(uint32_t addr, uint32_t value)
{
    spinor_burst_write(addr, (uint8_t*) &value, 4);
}

void spinor_preinit(void)
{
    nrf_gpio_pin_set(CONFIG_SPINOR_CS);
    nrf_gpio_cfg_output(CONFIG_SPINOR_CS);
}

void spinor_init(void)
{
    /* release deep power-down (RDP) */
    nrf_gpio_pin_clear(CONFIG_SPINOR_CS);
    spi_write(RDP, sizeof(RDP));
    nrf_gpio_pin_set(CONFIG_SPINOR_CS);
    NRFX_DELAY_US(1000);
}

#endif /* CONFIG_HAVE_SPINOR */
