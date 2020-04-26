/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#ifndef RELOADER_ST7789_H__
#define RELOADER_ST7789_H__

enum {
	STATE_BOOTLOADER_STARTED,
	STATE_BLE_DISCONNECTED,
	STATE_BLE_CONNECTED
};

void st7789_state(int state);
void st7789_init(void);
void st7789_teardown(void);

#endif /* RELOADER_ST7789_H__ */
