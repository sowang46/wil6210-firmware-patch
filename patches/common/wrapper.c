/***************************************************************************
 *                                                                         *
 *          ###########   ###########   ##########    ##########           *
 *         ############  ############  ############  ############          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ##            ##            ##   ##   ##  ##        ##          *
 *         ###########   ####  ######  ##   ##   ##  ##    ######          *
 *          ###########  ####  #       ##   ##   ##  ##    #    #          *
 *                   ##  ##    ######  ##   ##   ##  ##    #    #          *
 *                   ##  ##    #       ##   ##   ##  ##    #    #          *
 *         ############  ##### ######  ##   ##   ##  ##### ######          *
 *         ###########    ###########  ##   ##   ##   ##########           *
 *                                                                         *
 *            S E C U R E   M O B I L E   N E T W O R K I N G              *
 *                                                                         *
 * This file is part of NexMon.                                            *
 *                                                                         *
 * Copyright (c) 2016 NexMon Team                                          *
 *                                                                         *
 * NexMon is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * NexMon is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with NexMon. If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                         *
 **************************************************************************/

#ifndef WRAPPER_C
#define WRAPPER_C

#include <firmware_version.h>
#include <structs.h>
#include <patcher.h>
#include <helper.h>

#ifndef WRAPPER_H
    // if this file is not included in the wrapper.h file, create dummy functions
    #define VOID_DUMMY { ; }
    #define RETURN_DUMMY { ; return 0; }

    #define AT(CHIPVER, FWVER, ADDR) __attribute__((at(ADDR, "dummy", CHIPVER, FWVER)))
#else
    // if this file is included in the wrapper.h file, create prototypes
    #define VOID_DUMMY ;
    #define RETURN_DUMMY ;
    #define AT(CHIPVER, FWVER, ADDR)
#endif

AT(CHIP_VER_WIL6210, FW_VER_410_55, FW_ADDR(0x28828))
void
sub_28828(void)
VOID_DUMMY

AT(CHIP_VER_WIL6210, FW_VER_520_18, FW_ADDR(0x30040))
void
sub_30040(void)
VOID_DUMMY

AT(CHIP_VER_WIL6210, FW_VER_410_55, UC_ADDR(0x1604))
void
sub_1604(void)
VOID_DUMMY

AT(CHIP_VER_WIL6210, FW_VER_520_18, UC_ADDR(0x8bc))
void
sub_8bc(void)
VOID_DUMMY

AT(CHIP_VER_WIL6210, FW_VER_520_18, UC_ADDR(0x7e60))
int16_t
sub_7e60(void)
RETURN_DUMMY

AT(CHIP_VER_WIL6210, FW_VER_410_55, UC_ADDR(0xA170))
void
sub_A170()
VOID_DUMMY

AT(CHIP_VER_WIL6210, FW_VER_410_55, FW_ADDR(0x2B8))
void
store_r13_to_r17(void)
VOID_DUMMY

#undef VOID_DUMMY
#undef RETURN_DUMMY
#undef AT

#endif /*WRAPPER_C*/
