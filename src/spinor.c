/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#include "spinor.h"

#include <nrf_gpio.h>

#include "board.h"
#include "spi.h"

#ifdef CONFIG_HAVE_SPINOR

uint32_t spinor_page_size_get(void)
{
    return 0;
}

int spinor_page_erase(uint32_t addr)
{
    return -1;
}

void spinor_word_write(uint32_t addr, uint32_t value)
{
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
    spi_write((const uint8_t*) "\xab", 1);
    nrf_gpio_pin_set(CONFIG_SPINOR_CS);
}

#endif /* CONFIG_HAVE_SPINOR */
