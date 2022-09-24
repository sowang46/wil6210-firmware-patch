#pragma NEXMON targetregion "fwpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

__attribute__((at(FW_ADDR(0xd3ba), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked)) void b_fast_load_cb(void) {
    asm("ldw_s      r1,[r15,0]");
    fast_load_cb();
}

__attribute__((at(FW_ADDR(0x14128), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
void hwd_rfc_write_sector(void) { ; }

__attribute__((at(FW_ADDR(0xd3c0), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
void resume(void) { ; }