/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#include "flash.h"

#include <string.h>

#include <nrfx_nvmc.h>
#include <nrf_gpio.h>
#include <nrf_wdt.h>

#include "bootloader.h"
#include "util.h"
#include "wdt.h"

static int percent_complete;
static uint32_t bytes_processed;
static uint32_t total_bytes;

static void progress(int32_t nbytes)
{
    bytes_processed += nbytes;

    int percent = (bytes_processed * 100) / total_bytes;
    if (percent != percent_complete) {
	report_progress(percent);
	percent_complete = percent;
    }
}

/* slow but no table overhead ;-) */
uint32_t crc32(const uint8_t *data, uint32_t len)
{
	uint32_t crc = 0xffffffff;

	for (int i=0; i<len; i++) {
		crc = crc ^ data[i];
		for (int j=0; j<8; j++)
			crc = (crc >> 1) ^ (0xedb88320 & -(crc & 1));
	}
	return ~crc;
}

static uint32_t check_segment(const struct segment *sg)
{
    uint32_t sz = sg->end - sg->start;

    if (sg->crc32 != crc32(sg->data, sz))
	panic();

    return sz;
}

static void flash_segment(const struct segment *sg)
{
    uint32_t pagesz = nrfx_nvmc_flash_page_size_get();
    uint32_t sz = sg->end - sg->start;
    int retries = 5;

retry:
    
    /* TODO: Haven't got code to handle the UICR yet */
    if (sg->start >= 0x1000000) {
	progress(2 * sz);
	return;
    }

    for (uint32_t addr = sg->start; addr < sg->end; addr += pagesz) {
	nrf_wdt_reload_request_set(NRF_WDT, 0);
	nrfx_nvmc_page_erase(addr);
	progress(pagesz);
    }

    for (uint32_t addr = sg->start; addr < sg->end; addr += 4) {
	nrf_wdt_reload_request_set(NRF_WDT, 0);
	nrfx_nvmc_word_write(addr,
		             ((uint32_t *) sg->data)[(addr - sg->start) / 4]);
	progress(4);
    }

    if (0 != memcmp((void *) sg->start, sg->data, sz)) {
	progress(-(2 * sz));
	if (retries--)
	    goto retry;

	// We're in big trouble... the system is probably bricked but we
	// don't seem to be able to do anything about it. Better to reboot
	// and hope than to sit here wearing out the flash!
	panic();
    }
}

void flash_all(void)
{
    percent_complete = 0;
    bytes_processed = 0;
    total_bytes = 0;

    report_progress(0);

    for (int i=0; i<lengthof(segments); i++)
	total_bytes += 2 * check_segment(segments + i);
	
    for (int i=0; i<lengthof(segments); i++)
        flash_segment(segments + i);
}
