/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

_flash_size = 0x78000; /* bootloader starts at 0x78000 */
_ram_size = 64K;
_sd_size = 0x00026000;
_sd_ram  = 0x000039c0;
_app_size      = _flash_size - _sd_size;
_app_start     = _sd_size;
_app_ram_start = 0x20000000 + _sd_ram;
_app_ram_size  = _ram_size - _sd_ram;

MEMORY
{
  FLASH (rx) : ORIGIN = _app_start, LENGTH = _app_size
  RAM (rwx) : ORIGIN = _app_ram_start, LENGTH = _app_ram_size
  /*CODE_RAM (rwx) : ORIGIN = 0x800000, LENGTH = 0x10000*/
}

INCLUDE "nrf_common.ld"
