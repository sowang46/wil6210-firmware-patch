#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>
#include "ver.h"

#define BEAM_NUM 64
#define BUFFER_ADDR 0x1f300
#define MAC_STORE_ADDR 0x1f500
// #define TIMESTAMP_ADDR 0x1f600
#define STACK_ADDR "0x1f300"
#define STORE_REG(r, i) \
    asm("st " #r ", [" STACK_ADDR "-0x4*" #i "]");
#define LOAD_REG(r, i) \
    asm("ld " #r ", [" STACK_ADDR "-0x4*" #i "]");

void beacon_rss_common() {
    // Push registers in use to stack.
    // Make sure this patch won't modify those register values.
    STORE_REG(r2, 3)
    STORE_REG(r3, 4)
    STORE_REG(r4, 5)
    STORE_REG(r5, 6)
    STORE_REG(r6, 7)
    STORE_REG(r7, 8)
    
    // Check frame type and beam index of beacon frame.
    unsigned int *frame_start = (unsigned int *)0x00804010;
    unsigned int frame_type = (*frame_start)&0xffff;
    if (frame_type != 0x000c) {
        goto quit;
    }

    // Get beam index
    unsigned int beam_index = *(frame_start+4);
    beam_index >>= 0x1a;
    
    // Calculate the memory storage position for the beam index.
    // unsigned int ptr_offset = beam_index * (STORE_SIZE+4);
    unsigned int ptr_offset = beam_index;
    unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
    store_addr += ptr_offset;
    
    // Write RSS
    unsigned long *rss_ptr = store_addr;
    unsigned long *rss = (unsigned long *)0x00883BEC;
    
    // Write SNR
    register int *snr asm ("r6");
    asm("lsr r5, r40, 9");
    asm("bmsk r5, r5, 0xA");
    asm("sub r6, r5, 0x800");
    asm("cmp r5, 0x3FF");
    asm("mov.ls  r6, r5");
    
    *rss_ptr = ((*rss) & 0xFFFFF) | (((unsigned long)snr) << 20);

    // Fetch sender MAC address
    unsigned int mac_addr = *(frame_start+1);
    unsigned long *mac_store_addr = (unsigned long *)MAC_STORE_ADDR;
    mac_store_addr += ptr_offset;
    *mac_store_addr = mac_addr;

    // // Fetch frame timestamp
    // unsigned int time_stamp = (((*(frame_start+2)) & 0xffff0000) >> 16) | 
    //                           (((*(frame_start+3)) & 0x0000ffff) << 16);
    // unsigned long *timestamp_addr = (unsigned long *)TIMESTAMP_ADDR;
    // timestamp_addr += ptr_offset;
    // *timestamp_addr = time_stamp;
    
quit:
    // Restore the register values from the stack.
    LOAD_REG(r2, 3)
    LOAD_REG(r3, 4)
    LOAD_REG(r4, 5)
    LOAD_REG(r5, 6)
    LOAD_REG(r6, 7)
    LOAD_REG(r7, 8)
}

// -----------------------------Manage Mode injections---------------------------
// void beacon_rss_c1() {
//     beacon_rss_common();
//     // beacon_rss_mac();
//     asm("pop_s blink");
//     // Resume the function in the injection point.
//     asm("j 0x1e7b6");
// }

// // Middle of wait_dmg_beacons_during_bti
// __attribute__((at(UC_ADDR(0x1e7b2), "", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
// __attribute__((naked)) void b_beacon_rss_b1(void) {
//     beacon_rss_c1();
// }

// void beacon_rss_c2() {
//     beacon_rss_common();
//     // beacon_rss_mac();
//     asm("pop_s blink");
//     // Resume the function in the injection point.
//     asm("j 0x1715e");
// }

// // Middle of tbtt_wait_1st_dmg_beacon_or_bi2
// __attribute__((at(UC_ADDR(0x1715a), "", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
// __attribute__((naked)) void b_beacon_rss_b2(void) {
//     beacon_rss_c2();
// }

// //Patch version information
// __attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
// __attribute__((naked))
// void str_ver(void) { 
//    asm(".ascii \"Patched: v2x_client2 (ver: 0.1." BUILD_VER ")\""); 
// }




// -----------------------------Monitor mode injections (testing)---------------------------
void beacon_rss_c1() {
    beacon_rss_common();
    asm("pop_s blink");
    // Resume the function in the injection point.
    asm("bclr r25,r25,10");
    asm("j 0x3984");
}

// Middle of bti_pending_beacon_reception
__attribute__((at(UC_ADDR(0x3980), "", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
__attribute__((naked)) void b_beacon_rss_b1(void) {
    beacon_rss_c1();
}

void beacon_rss_c2() {
    beacon_rss_common();
    asm("pop_s blink");
    // Resume the function in the injection point.
    asm("mov r0,5");
    asm("j 0xb2c0");
}

// Exit of inject_busy_event_10bi_manager
__attribute__((at(UC_ADDR(0xb2bc), "", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
__attribute__((naked)) void b_beacon_rss_b2(void) {
    beacon_rss_c2();
}

// Patch version information
__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40_PLUS)))
__attribute__((naked))
void str_ver(void) { 
    asm(".ascii \"Patched: v2x_client_test (ver: 0.1." BUILD_VER ")\n\""); 
}
