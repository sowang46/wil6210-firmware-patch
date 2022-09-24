#pragma NEXMON targetregion "fwpatch"

#include <firmware_version.h>
#include <patcher.h>

// Fast codebook patch
// void fast_load_cb();

// __attribute__((at(FW_ADDR(0xd618), "", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
// __attribute__((naked)) void b_fast_load_cb(void) {
//     asm("ldw_s      r1,[r15,0]");
//     fast_load_cb();
// }

// __attribute__((at(FW_ADDR(0x14e28), "dummy", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
// void hwd_rfc_write_sector(void) { ; }

// __attribute__((at(FW_ADDR(0xf268), "dummy", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
// void hwd_abif_dvs_rfca_cmd_enable(void) { ; }

// __attribute__((at(FW_ADDR(0xd61e), "dummy", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
// void resume(void) { ; }