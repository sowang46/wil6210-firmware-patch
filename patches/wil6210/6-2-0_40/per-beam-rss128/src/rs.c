#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

// __attribute__((at(UC_ADDR(0x3ab8+0x4), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
// void resume_1(void) { ; }


// __attribute__((at(UC_ADDR(0x39ae), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
// void resume_2(void) { ; }


// __attribute__((at(UC_ADDR(0x39a4), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
// void resume_3(void) { ; }

// __attribute__((at(UC_ADDR(0x3990), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
// void resume_4(void) { ; }

// __attribute__((at(UC_ADDR(0x1080c), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
// void resume_5(void) { ; }

// __attribute__((at(UC_ADDR(0x3a74), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
// void process_bti(void) { ; }