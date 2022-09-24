#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include "ver.h"

#define STORE_SIZE 12
#define BEAM_NUM 64
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
    beam_index >>= 0x1a;
    
    // Calculate the memory storage position for the beam index.
    unsigned int ptr_offset = beam_index * (STORE_SIZE+4);
    unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
    store_addr += ptr_offset + 4;
    
    // Update packet counter for the beam index.
    unsigned long *counter_ptr =  store_addr - 4;
    (*counter_ptr)++;
    
    // Check if the timestamp of the frame is the same as the previously 
    // received one. The injection may be called multiple times for the same
    // received frame.
    unsigned int time_stamp = (((*(frame_start+2))&0xffff0000) >> 16) | 
                              (((*(frame_start+3))&0x0000ffff) << 16);
    unsigned long *timestamp_ptr = store_addr - 3;
    if (*timestamp_ptr == time_stamp) {
        goto quit;
    } else {
        *timestamp_ptr = time_stamp;
    }
    
    // Head pointer of the ring buffer.
    unsigned long **head_ptr =  (unsigned long **)(store_addr - 2);
    if (*head_ptr == 0) {
        *head_ptr = store_addr;
    }
    
    // Write RSS to the head
    unsigned long *target = *head_ptr;
    unsigned long *rss = (unsigned long *)0x00883BEC;
    *target = *rss;
    
    // Write SNR to the head+offset
    register int *snr asm ("r6");
    asm("lsr r5, r40, 9");
    asm("bmsk r5, r5, 0xA");
    asm("sub r6, r5, 0x800");
    asm("cmp r5, 0x3FF");
    asm("mov.ls  r6, r5");
    *(target+STORE_SIZE/2) = (unsigned long)snr;
    
    // Advance the pointer of ring buffer. Rewind if necessary
    *head_ptr = *head_ptr + 1;
    if (*head_ptr - store_addr >= STORE_SIZE/2) {
        *head_ptr = store_addr;
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
    // Resume the function in the injection point.
    asm("j 0x1df6e");
}

// Middle of wait_dmg_beacons_during_bti
__attribute__((at(UC_ADDR(0x1df6a), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked)) void b_beacon_rss_b1(void) {
    beacon_rss_c1();
}

void beacon_rss_c2() {
    beacon_rss_common();
    asm("pop_s blink");
    // Resume the function in the injection point.
    asm("j 0x169a6");
}

// Middle of tbtt_wait_1st_dmg_beacon_or_bi2
__attribute__((at(UC_ADDR(0x169a2), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked)) void b_beacon_rss_b2(void) {
    beacon_rss_c2();
}

// Patch version information
__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked))
void str_ver(void) { 
    asm(".ascii \"Patched: per-beam-rss3 (ver: 3.1." BUILD_VER ")\n\""); 
}
