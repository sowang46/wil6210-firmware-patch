#pragma NEXMON targetregion "fwpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

/*
 * Hook the initialization function and print debug output
 */
void
fw_hook() {
    printf("FW: INITIALIZED 620_40\n");
    fw_sub_3003C();
}

__attribute__((at(FW_ADDR(0x1770), "", CHIP_VER_WIL6210, FW_VER_620_40)))
BLPatch(fw_hook, fw_hook);
