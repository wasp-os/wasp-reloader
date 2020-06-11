/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#include <stdint.h>
#include <nrf_gpio.h>

#include "board.h"
#include "flash.h"
#include "st7789.h"
#include "util.h"
#include "wdt.h"

void panic(void)
{
#ifdef CONFIG_ST7789
    st7789_state(-1);
#endif
    reboot();
}

void reboot(void)
{
    while (true)
	    /* stop feeding the dog and we'll automatically reboot */;
}

void report_progress(int percent)
{
#ifdef CONFIG_ST7789
    st7789_state(percent);
#else
    nrf_gpio_pin_set(CONFIG_USR_LED);
    for(volatile int c = 0; c < 250000; c++);
    nrf_gpio_pin_clear(CONFIG_USR_LED);
    for(volatile int c = 0; c < 250000; c++);
#endif
    wdt_feed(false);
}

int main()
{
    wdt_init();

#ifdef CONFIG_ST7789
    st7789_init();
#else
    nrf_gpio_cfg_output(CONFIG_USR_LED);
#endif

    flash_all();
    reboot();

    return 0;
}
