/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#include "wdt.h"

#include <nrf_gpio.h>
#include <nrf_wdt.h>

#include "board.h"

#if CONFIG_HAVE_WDT_BUTTON
static void button_init(void)
{
    nrf_gpio_cfg_sense_input(
            CONFIG_WDT_BUTTON,
            CONFIG_WDT_BUTTON_PULL < 0 ? NRF_GPIO_PIN_PULLDOWN :
            CONFIG_WDT_BUTTON_PULL > 0 ? NRF_GPIO_PIN_PULLUP :
                                             NRF_GPIO_PIN_NOPULL,
            NRF_GPIO_PIN_NOSENSE);

#if CONFIG_WDT_BUTTON_ENABLE
    nrf_gpio_cfg_output(CONFIG_WDT_BUTTON_ENABLE);
    nrf_gpio_pin_write(CONFIG_WDT_BUTTON_ENABLE,
                       CONFIG_WDT_BUTTON_ACTIVE);
#endif
}

static bool button_pressed(void)
{
    return nrf_gpio_pin_read(CONFIG_WDT_BUTTON) == CONFIG_WDT_BUTTON_ACTIVE;
}
#endif

void wdt_init(void)
{
    if (!nrf_wdt_started(NRF_WDT)) {
        // 1 => keep running during a sleep, stop during SWD debug
        nrf_wdt_behaviour_set(NRF_WDT, 1);

        // timeout after 5 seconds
        nrf_wdt_reload_value_set(NRF_WDT, 5 * 32768);

        // enable the 0th channel
        nrf_wdt_reload_request_enable(NRF_WDT, NRF_WDT_RR0);

        // set it running
        nrf_wdt_task_trigger(NRF_WDT, NRF_WDT_TASK_START);

#if CONFIG_HAVE_WDT_BUTTON
	// if there was no bootloader to configure the WDT then it
	// probably didn't setup the pins for the button either
	button_init();
#endif
    }
}

void wdt_feed(bool isr)
{
    /*
     * A WDT button allows us to feed the dog from somewhere that would
     * normally be "silly", such as a periodic timer interrupt. By providing
     * the user direct control over feeding the dog we are, in effect,
     * implementing a (reasonably robust) long-press reset button.
     */
#if CONFIG_HAVE_WDT_BUTTON
    if (!button_pressed())
#else
    if (!isr)
#endif
        nrf_wdt_reload_request_set(NRF_WDT, 0);
}
