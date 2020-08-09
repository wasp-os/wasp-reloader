/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#ifndef RELOADER_BOARD_K9_H__
#define RELOADER_BOARD_K9_H__

#define CONFIG_ST7789
#define CONFIG_ST7789_BACKLIGHT  (22)

// WDT config
#define CONFIG_HAVE_WDT_BUTTON   (1)
#define CONFIG_WDT_BUTTON        (9)
#define CONFIG_WDT_BUTTON_ACTIVE (1) // active high
#define CONFIG_WDT_BUTTON_PULL   (0) // no pull up/down

#endif /* RELOADER_BOARD_K9_H__ */
