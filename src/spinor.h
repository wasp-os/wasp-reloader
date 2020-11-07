/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#ifndef RELOADER_SPINOR_H__
#define RELOADER_SPINOR_H__

#include <stdint.h>

uint32_t spinor_page_size_get(void);
int spinor_page_erase(uint32_t addr);
void spinor_word_write(uint32_t addr, uint32_t value);
void spinor_preinit(void);
void spinor_init(void);

#endif /* RELOADER_SPINOR_H__ */
