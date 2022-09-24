#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include "ver.h"


// ROM:000182A8                 or      r0, r0, r1
// ROM:000182AA                 asl     r2, r2, 0x17
// ROM:000182AC                 or      r0, r0, r2
// ROM:000182AE                 st      r0, [gp,0x90]
// ROM:000182B2                 beq     loc_182D6
// ROM:000182B4                 ld      r0, [sp,0x54+var_14]
// ROM:000182B6                 st      loc_19000, [sp,0x54+var_C]
// ROM:000182BE                 bset    r0, r0, 0
// ROM:000182C0                 st      r0, [sp,0x54+var_14]
// ROM:000182C2                 mov     r0, 0
// ROM:000182C4                 st      r0, [sp,0x54+var_24]
// ROM:000182C6                 st      r0, [sp,0x54+var_20]
// ROM:000182C8                 bl.d    0x16DB0
// ROM:000182CC                 add     r0, sp, 0x54+var_24
// ROM:000182CE                 mov     r0, r16
// ROM:000182D0                 bl.d    0xAC3C
// ROM:000182D4                 mov     r1, r13


void bi_overwrite() {
    unsigned int *bi_addr = (unsigned int *)0x0093eb00;
    //if (*bi_addr == 0) {
    //    *bi_addr = 102400;
    //    //*bi_addr = 15360;
    //}
    *bi_addr = 1024*10;
    asm("ld r2, [0x0093eb00]");
    asm("st r2,[sp,72]");
    asm("j 0x182BE");
}

//__attribute__((at(UC_ADDR(0x182B6), "", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
//BPatch(bi_overwrite, bi_overwrite);

// Patch version information
//__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
//__attribute__((naked))
//void str_ver(void) { 
//    asm(".ascii \"Patched: ap-fast-beacon (ver: 0.2." BUILD_VER ")\""); 
//}
