/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#ifndef RELOADER_BOARD_NITROGEN_H__
#define RELOADER_BOARD_NITROGEN_H__

#define CONFIG_USR_LED 29

// WDT config
#define CONFIG_HAVE_WDT_BUTTON   (1)
#define CONFIG_WDT_BUTTON        (27)
#define CONFIG_WDT_BUTTON_ACTIVE (0) // active low
#define CONFIG_WDT_BUTTON_PULL   (1)  // pull up
#define CONFIG_WDT_BUTTON_ENABLE (0) // no enable pin

#endif /* RELOADER_BOARD_NITROGEN_H__ */
