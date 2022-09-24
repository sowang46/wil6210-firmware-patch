#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>

void uc_cir_call() {
    asm("cmp_s r1, 0");
    asm("jls [blink]");
    asm("mov lp_count, 0xf");
    asm("mov r3, 0x00883844");
    asm("lp 0xc");
    asm("ld.ab r2, [r3,4]");
    asm("st.ab r2, [r0,4]");
}

__attribute__((at(UC_ADDR(0x9BD8), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((optimize("O0")))
// BPatch(uc_cir_call, uc_cir_call);
// JPatch(uc_cir_call, uc_cir_call);
// GenericPatch4(uc_cir_call, 0x0ac00401);
GenericPatch4(uc_cir_call, uc_cir_call+1);
// Dummy(uc_cir_call);

// __attribute__((at(UC_ADDR(0x9BDC), "", CHIP_VER_WIL6210, FW_VER_620_40)))
// GenericPatch2(uc_cir_call_noreturn, 0xe180);
