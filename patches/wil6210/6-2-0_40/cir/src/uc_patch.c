#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

/*
 * Hook the initialization function and print debug output
 */
void
uc_hook() {
    printf("UC: INITIALIZED 620_40 v11\n");
    uc_sub_97C();
}

__attribute__((at(UC_ADDR(0x8BC), "", CHIP_VER_WIL6210, FW_VER_620_40)))
BLPatch(uc_hook, uc_hook);