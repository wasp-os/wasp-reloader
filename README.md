wasp-reloader: Re-flash utility for wasp-os
===========================================

Introduction
------------

wasp-reloader is a tool that is used to update to the latest version
of the wasp-bootloader.

It is a application payload that, when executed, will replace the
softdevice and bootloader with an updated version and then reboot
into the new bootloader ready to receive a new payload.

Building wasp-reloader
----------------------

Note: *wasp-reloader is tested using the [GNU-RM toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm) (9-2019-q4) from Arm.*

1. Use `hex2c.py` (from wasp-os `tools/` directory) to populate
   `src/boards/<boardname>/bootloader.h`
2. `git submodule init --update`
3. `make`
4. Deploy!

Status
------

 * Can replace MBR and softdevice with different versions.
 * Can replace the wasp-bootloader binary.
 * Can be used as an OTA upgrade to replace the PineTime factory
   default bootloader (mcuboot) with wasp-bootloader.

License
-------

wasp-reloader program is free software: you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License and GNU Lesser General Public License for more
details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
