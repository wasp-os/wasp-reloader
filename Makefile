# SPDX-License-Identifier: LGPL-3.0-or-later
# Copyright (C) 2020 Daniel Thompson

BOARD ?= pinetime
CROSS_COMPILE := arm-none-eabi-

CC = $(CROSS_COMPILE)gcc
CFLAGS += \
	  -mcpu=cortex-m4 -mthumb -g3 -Os \
	  -ffunction-sections \
	  -fdata-sections \
	  -DNRF52832_XXAA \
	  -Ilib/CMSIS/CMSIS/Core/Include \
	  -Ilib/nrfx \
	  -Ilib/nrfx/drivers/include \
	  -Ilib/nrfx/mdk \
	  -Ilib/nrfx/hal \
	  -Isrc/boards/$(BOARD) -Isrc
LDFLAGS += $(CFLAGS) -Wl,-Map=$(OBJDIR)/reloader.map -Lsrc -Llib/nrfx/mdk -T$(LDSCRIPT)

APP = $(OBJDIR)/reloader.zip
OBJS = main.o \
       flash.o \
       st7789.o \
       wdt.o \
       nrfx_nvmc.o \
       gcc_startup_nrf52.o \
       system_nrf52.o
OBJDIR = build-$(BOARD)
OBJS := $(OBJS:%.o=$(OBJDIR)/%.o)

all: $(APP)
$(OBJDIR)/reloader.elf: LDSCRIPT = nrf52832_xxaa.ld
$(OBJDIR)/reloader.elf: $(OBJDIR) $(OBJS) $(LDFILE)
	$(CROSS_COMPILE)gcc $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)/reloader.hex: $(OBJDIR)/reloader.elf
	$(CROSS_COMPILE)objcopy -O ihex $< $@

$(OBJDIR)/reloader.zip : $(OBJDIR)/reloader.hex
	python3 -m nordicsemi dfu genpkg --dev-type 0x0052 --application $< $@

ifeq ($(BOARD),pinetime)
all : $(OBJDIR)/reloader-mcuboot.zip $(OBJDIR)/reloader-factory.zip
endif

$(OBJDIR)/reloader-mcuboot.elf: LDSCRIPT = nrf52832_xxaa_mcuboot.ld
$(OBJDIR)/reloader-mcuboot.elf: $(OBJDIR) $(OBJS) $(LDFILE)
	$(CROSS_COMPILE)gcc $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)/reloader-mcuboot.bin: $(OBJDIR)/reloader-mcuboot.elf
	$(CROSS_COMPILE)objcopy -O binary $< $@.tmp
	lib/mcuboot/scripts/imgtool.py create \
		--align 4 --version 1.0.0 \
		--header-size 32 --slot-size 475136 --pad-header $@.tmp $@
	$(RM) $@.tmp

$(OBJDIR)/reloader-mcuboot.zip : $(OBJDIR)/reloader-mcuboot.bin
	python3 -m nordicsemi dfu genpkg --dev-type 0x0052 --application $< $@

FACTORY_OBJS = $(subst flash.o,factoryflash.o,$(OBJS))
$(OBJDIR)/reloader-factory.elf: LDSCRIPT = nrf52832_xxaa.ld
$(OBJDIR)/reloader-factory.elf: $(OBJDIR) $(FACTORY_OBJS) $(LDFILE)
	$(CROSS_COMPILE)gcc $(LDFLAGS) $(FACTORY_OBJS) -o $@

$(OBJDIR)/reloader-factory.hex: $(OBJDIR)/reloader-factory.elf
	$(CROSS_COMPILE)objcopy -O ihex $< $@

$(OBJDIR)/reloader-factory.zip : $(OBJDIR)/reloader-factory.hex
	python3 -m nordicsemi dfu genpkg --dev-type 0x0052 --application $< $@

VPATH = src:lib/nrfx/drivers/src:lib/nrfx/mdk

$(OBJDIR) :
	mkdir -p $@

$(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/factoryflash.o : flash.c
	$(CC) $(CFLAGS) -DFACTORY -c $< -o $@

$(OBJDIR)/%.o : %.S
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r $(OBJDIR)
