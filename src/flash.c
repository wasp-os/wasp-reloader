/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright (C) 2020 Daniel Thompson
 */

#include "flash.h"

#include <nrfx_nvmc.h>
#include <nrf_gpio.h>
#include <nrf_wdt.h>

#include "bootloader.h"
#include "util.h"
#include "wdt.h"

static int percent_complete;
static uint32_t bytes_processed;
static uint32_t total_bytes;

static void progress(uint32_t nbytes)
{
    bytes_processed += nbytes;

    int percent = (bytes_processed * 100) / total_bytes;
    if (percent != percent_complete) {
	report_progress(percent);
	percent_complete = percent;
    }
}

static void flash_segment(const struct segment *sg)
{
    uint32_t pagesz = nrfx_nvmc_flash_page_size_get();
    
    /* TODO: Haven't got code to handle the UICR yet */
    if (sg->start >= 0x1000000) {
	progress(sg->end - sg->start);
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
}

void flash_all(void)
{
    percent_complete = 0;
    bytes_processed = 0;
    total_bytes = 0;

    report_progress(0);

    for (int i=0; i<lengthof(segments); i++)
	total_bytes += 2 * (segments[i].end - segments[i].start);
	
    for (int i=0; i<lengthof(segments); i++)
        flash_segment(segments + i);
}
