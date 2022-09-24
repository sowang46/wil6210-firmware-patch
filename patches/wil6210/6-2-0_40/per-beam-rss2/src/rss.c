#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>
#include "ver.h"

#define STORE_SIZE 28
#define BEAM_NUM 64
#define BUFFER_ADDR 0x008FC000

void beacon_rss() {
    asm("push r2");
    asm("push r3");
    asm("push r4");
    asm("push r5");
    asm("push r6");
    asm("push r7");
    asm("push r8");
    
    unsigned int *frame_start = (unsigned int *)0x00804010;
    // unsigned long *rss = (unsigned long *)0x00883BEC;
    unsigned int beam_index = *(frame_start+4);
    beam_index = beam_index >> 0x1a;
    unsigned int frame_type = (*frame_start)&0xffff;
    unsigned int time_stamp = (((*(frame_start+2))&0xffff0000) >> 16) | \
                              (((*(frame_start+3))&0x0000ffff) << 16);
    
    if (frame_type != 0x000c || beam_index >= BEAM_NUM) {
        goto quit;
    }
    
    unsigned int ptr_offset = beam_index * (STORE_SIZE+4);
    unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
    store_addr += ptr_offset + 4;
    
    unsigned long *counter_ptr =  store_addr - 4;
    unsigned long *overflow_ptr = store_addr - 3;
    unsigned long **head_ptr =  (unsigned long **)(store_addr - 2);
    unsigned long **tail_ptr =  (unsigned long **)(store_addr - 1);
    
    if (*head_ptr == 0|| *tail_ptr == 0) {
        *head_ptr = store_addr;
        *tail_ptr = store_addr;
    }
    
    // (*counter_ptr)++;
    if (*counter_ptr == time_stamp) {
        goto quit;
    }
    *counter_ptr = time_stamp;
    
    unsigned long *next_head_ptr = *head_ptr + 1;
    if (next_head_ptr - store_addr >= STORE_SIZE) {
        next_head_ptr = store_addr;
    }
    
    if (next_head_ptr != *tail_ptr) {
        unsigned long *target = *head_ptr;
        // *target = *rss;
        *target = time_stamp;
        *head_ptr = next_head_ptr;
    } else {
        // ring buffer overflows;
        *overflow_ptr = 1;
    }
    
quit:
    asm("pop r8");
    asm("pop r7");
    asm("pop r6");
    asm("pop r5");
    asm("pop r4");
    asm("pop r3");
    asm("pop r2");
    
    // asm("ld         blink,[sp,4]");
    // asm("j 0x1108c");
    
    // asm("bclr       r25,r25,10");
    // asm("j 0x16854");
    
    // asm("mov        r16,0x140");
    // asm("j 0x1def0");
    
    // asm("jl 0x254");
    // asm("j 0x38c2");
    
    // asm("ld.as      r2,[gp,60]");
    // asm("j 0x10e74");
    
    // asm("jl 0xefc0");
    // asm("j 0x11ce0");
    
    // asm("mov        r9,r42");
    // asm("j 0x118d8");
    
    asm("ld         blink,[sp,4]");
    asm("j 0x1283e");
    
    // asm("jl 0xefc0");
    // asm("j 0x11d94");
    
    // asm("jl 0xefc0");
    // asm("j 0x11f4c");
    
    // asm("jl 0xefc0");
    // asm("j 0x11f68");
    
    // asm("mov        r32,r3");
    // asm("j 0x12810");
    
    // asm("mov_s      r1,r36");
    // asm("btst_s     r1,25");
    // asm("j 0x11a1c");
    
    // asm("extb.f     0,r2");
    // asm("j 0x11c88");
}

// 10688	sub_FDA8	<->	11088	sub_10798	<->			rx_flow(void)
// __attribute__((at(UC_ADDR(0x11088), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 15b88	sub_15B88	<->	16830	sub_1682C	<->			bti_worker::tbtt_wait_1st_dmg_beacon_or_bi2((void))
// __attribute__((at(UC_ADDR(0x16850), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 1d0c4	sub_1D0C4	<->	1ded8	sub_1DED4	<->			bti_worker::wait_dmg_beacons_during_bti((void))
// __attribute__((at(UC_ADDR(0x1deec), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 3c60	sub_3C60	<->	38be	sub_38BC	<->			bti_pending_beacon_reception(BTI_START_CAUSE,int)
// __attribute__((at(UC_ADDR(0x38be), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 10484	sub_FDA8	<->	10e70	sub_10798	<->			rx_flow(void)
// rx_flow_responder
// __attribute__((at(UC_ADDR(0x10e70), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 111b8	sub_10E38	<->	11ce0	sub_118B8	<->			rx_flow_responder(void)
// rfc_ctrl_disable_tx_rx_schemes, left1
// __attribute__((at(UC_ADDR(0x11cdc), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 10e50	sub_10E38	<->	118d4	sub_118B8	<->			rx_flow_responder(void)
// entrance of the function
// __attribute__((at(UC_ADDR(0x118d4), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 11d18	sub_10E38	<->	12842	sub_118B8	<->			rx_flow_responder(void)
// exit of the function
__attribute__((at(UC_ADDR(0x1283a), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 1125c	sub_10E38	<->	11d90	sub_118B8	<->			rx_flow_responder(void)
// rfc_ctrl_disable_tx_rx_schemes, left2
// __attribute__((at(UC_ADDR(0x11d90), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 11414	sub_10E38	<->	11f48	sub_118B8	<->			rx_flow_responder(void)
// rfc_ctrl_disable_tx_rx_schemes, left3
// __attribute__((at(UC_ADDR(0x11f48), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 11434	sub_10E38	<->	11f64	sub_118B8	<->			rx_flow_responder(void)
// rfc_ctrl_disable_tx_rx_schemes, left4
// __attribute__((at(UC_ADDR(0x11f64), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 11ce2	sub_10E38	<->	1280c	sub_118B8	<->			rx_flow_responder(void)
// __attribute__((at(UC_ADDR(0x1280c), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 10f00	sub_10E38	<->	11a18	sub_118B8	<->			rx_flow_responder(void)
// __attribute__((at(UC_ADDR(0x11a18), "", CHIP_VER_WIL6210, FW_VER_620_40)))

// 11164	sub_10E38	<->	11c80	sub_118B8	<->			rx_flow_responder(void)
// __attribute__((at(UC_ADDR(0x11c80), "", CHIP_VER_WIL6210, FW_VER_620_40)))


__attribute__((naked)) void b_beacon_rss(void) {
    beacon_rss();
}


__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked))
void str_ver(void) { 
    asm(".ascii \"Patched: per-beam-rss2 (ver: 2.1." BUILD_VER ")\n\""); 
}
