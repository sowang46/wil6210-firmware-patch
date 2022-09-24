#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include "ver.h"

#define STORE_SIZE 12 // number of samples
#define SAMPLE_SIZE 2
#define BUFFER_ADDR 0x1eb00
#define STACK_ADDR "0x1eb00"
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
    
    unsigned int beam_index = *(frame_start+4);
    unsigned int dmg_antenna = *(frame_start+5);
    beam_index >>= 0x1a;
    dmg_antenna &= 0x1;
    dmg_antenna <<= 0x6;
    beam_index += dmg_antenna;
    
    // unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
    // unsigned int ptr_offset = 0;
    // store_addr += ptr_offset + 3;
    // unsigned long **head_ptr =  (unsigned long **)(store_addr - 1);
    // if (*head_ptr == 0) {
    //     *head_ptr = store_addr;
    // }
    // unsigned long *target = *head_ptr;
    // *target = beam_index;
    // if (*head_ptr - store_addr <= 500) {
    //     *head_ptr = *head_ptr + 1;
    // }
    // goto quit;
    
    
    // Calculate the memory storage position for the beam index.
    unsigned int ptr_offset = beam_index * (STORE_SIZE*SAMPLE_SIZE/4+3);
    unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
    store_addr += ptr_offset + 3;
    
    // Check if the timestamp of the frame is the same as the previously 
    // received one. The injection may be called multiple times for the same
    // received frame.
    unsigned int time_stamp = (((*(frame_start+2))&0xffff0000) >> 16) | 
                              (((*(frame_start+3))&0x0000ffff) << 16);
    unsigned long *timestamp_ptr = store_addr - 2;
    if (*timestamp_ptr == time_stamp) {
        goto quit;
    } else {
        *timestamp_ptr = time_stamp;
    }
    
    // Update packet counter for the beam index.
    unsigned long *counter_ptr =  store_addr - 3;
    (*counter_ptr)++;
    
    // Head pointer of the ring buffer.
    unsigned short **head_ptr =  (unsigned short **)(store_addr - 1);
    if (*head_ptr == 0) {
        *head_ptr = (unsigned short *) store_addr;
    }
    
    // Write RSS to the head
    unsigned short *target = *head_ptr;
    unsigned long *rss = (unsigned long *)0x00883BEC;
    *target = (unsigned short) ((*rss)/16);
    // *target = (unsigned short) ((time_stamp/100)%0xffff);
    
    // Advance the pointer of ring buffer. Rewind if necessary
    *head_ptr = *head_ptr + 1;
    if (*head_ptr - (unsigned short *) store_addr >= STORE_SIZE) {
        *head_ptr = (unsigned short *) store_addr;
    }
    
quit:
    // Restore the register values from the stack.
    LOAD_REG(r2, 3)
    LOAD_REG(r3, 4)
    LOAD_REG(r4, 5)
    LOAD_REG(r5, 6)
    LOAD_REG(r6, 7)
    LOAD_REG(r7, 8)
}

void beacon_rss_c1() {
    beacon_rss_common();
    asm("pop_s blink");
    // Resume the function of the injection point.
    asm("bclr r25,r25,10");
    asm("j 0x3914");
}

// Middle of bti_pending_beacon_reception
__attribute__((at(UC_ADDR(0x3910), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked)) void b_beacon_rss_b1(void) {
    beacon_rss_c1();
}


void beacon_rss_c2() {
    beacon_rss_common();
    asm("pop_s blink");
    // Resume the function of the injection point.
    asm("mov r0,5");
    asm("j 0xafbc");
}

// Exit of inject_busy_event_10bi_manager
__attribute__((at(UC_ADDR(0xafb8), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked)) void b_beacon_rss_b2(void) {
    beacon_rss_c2();
}

// void beacon_rss_c1() {
//     beacon_rss_common();
//     asm("pop_s blink");
//     // Resume the function in the injection point.
//     asm("j 0x1df6e");
// }

// // Middle of wait_dmg_beacons_during_bti
// __attribute__((at(UC_ADDR(0x1df6a), "", CHIP_VER_WIL6210, FW_VER_620_40)))
// __attribute__((naked)) void b_beacon_rss_b1(void) {
//     beacon_rss_c1();
// }

// void beacon_rss_c2() {
//     beacon_rss_common();
//     asm("pop_s blink");
//     // Resume the function in the injection point.
//     asm("j 0x169a6");
// }

// // Middle of tbtt_wait_1st_dmg_beacon_or_bi2
// __attribute__((at(UC_ADDR(0x169a2), "", CHIP_VER_WIL6210, FW_VER_620_40)))
// __attribute__((naked)) void b_beacon_rss_b2(void) {
//     beacon_rss_c2();
// }

// Patch version information
__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked))
void str_ver(void) { 
    asm(".ascii \"Patched: per-beam-rss5 (ver: 5.1." BUILD_VER ")\"");
}
