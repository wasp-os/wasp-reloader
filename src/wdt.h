/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#ifndef RELOADER_WDT_H__
#define RELOADER_WDT_H__

#include <stdbool.h>

void wdt_init(void);
void wdt_feed(bool isr);

#endif /* RELOADER_WDT_H__ */
