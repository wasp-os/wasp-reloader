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
2. `make`
3. Deploy!

Status
------

 * Can replace MBR and softdevice with alternatives that are the same
   size (or smaller).
 * Can replace the bootloader binary.
 * Does not update the UICR (which contains the bootloader entry point)

The effect of this is that there are just enough features implemented
to deliver updates which do not change the flash partitioning for
the softdevice or bootloader.

This features could easily be implemented but since they would require
a different SoftDevice to test them that work hasn't taken place quite
yet.

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
