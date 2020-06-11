/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#ifndef RELOADER_UTIL_H__
#define RELOADER_UTIL_H__

#define lengthof(x) (sizeof(x) / sizeof((x)[0]))

void panic(void);
void reboot(void);
void report_progress(int percent);

#endif /* RELOADER_UTIL_H__ */
