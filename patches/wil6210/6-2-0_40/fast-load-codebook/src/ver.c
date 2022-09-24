#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked))
void str_ver(void) { 
    asm(".ascii \"Patched: fast-load-codebook (ver: 0.2.19)\n\""); 
}
