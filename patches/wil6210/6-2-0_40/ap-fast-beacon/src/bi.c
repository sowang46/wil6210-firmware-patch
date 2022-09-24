#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>

//   17ae2:       13 f2                           beq_s      0x17b06
//   17ae4:       10 c0                           ld_s       r0,[sp,64]

//   17ae6:       48 1c 80 3f 01 00 00 90         st         0x00019000,[sp,72]

//   17aee:       80 b8                           bset_s     r0,r0,0
//   17af0:       50 c0                           st_s       r0,[sp,64]
//   17af2:       00 d8                           mov_s      r0,0
//   17af4:       4c c0                           st_s       r0,[sp,48]
//   17af6:       4d c0                           st_s       r0,[sp,52]
//   17af8:       02 0b 6f ff                     bl.d       0x165f8

void bi_overwrite() {
    unsigned int *bi_addr = 0x0093eb00;
    //if (*bi_addr == 0) {
        //*bi_addr = 102400;
    *bi_addr = 5120;
    //}
    asm("ld r2, [0x0093eb00]");
    asm("st r2,[sp,72]");
    asm("j 0x17aee");
}
__attribute__((at(UC_ADDR(0x17ae6), "", CHIP_VER_WIL6210, FW_VER_620_40)))
BPatch(bi_overwrite, bi_overwrite);

// Patch version information
__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked))
void str_ver(void) { 
    asm(".ascii \"Patched: ap-fast-beacon (ver: 0.5)\"");
}
