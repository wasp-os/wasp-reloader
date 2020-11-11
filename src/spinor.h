/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#ifndef RELOADER_SPINOR_H__
#define RELOADER_SPINOR_H__

#include <stdint.h>

uint32_t spinor_page_size_get(void);
uint32_t spinor_sector_size_get(void);
void spinor_sector_erase(uint32_t addr);
void spinor_burst_write(uint32_t addr, const uint8_t *buf, uint32_t buflen);
void spinor_word_write(uint32_t addr, uint32_t value);
void spinor_preinit(void);
void spinor_init(void);

#endif /* RELOADER_SPINOR_H__ */
