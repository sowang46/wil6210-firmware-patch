#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>
#include "ver.h"

#define STORE_SIZE 12
#define BEAM_NUM 64
#define BUFFER_ADDR 0x008FDC00
#define SNR_ADDR_OFFSET 1024

void beacon_rss_common() {
    // Push registers in use to stack.
    // Make sure this patch won't modify those register values.
    asm("sub sp, sp, 0x20");
    asm("push r2");
    asm("push r3");
    asm("push r4");
    asm("push r5");
    asm("push r6");
    
    // Check frame type and beam index of beacon frame.
    unsigned int *frame_start = (unsigned int *)0x00804010;
    unsigned int beam_index = *(frame_start+4);
    beam_index = beam_index >> 0x1a;
    unsigned int frame_type = (*frame_start)&0xffff;
    
    if (frame_type != 0x000c || beam_index >= BEAM_NUM) {
        goto quit;
    }
    
    // Calculate the memory storage position for the beam index.
    unsigned int ptr_offset = beam_index * (STORE_SIZE+4);
    unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
    store_addr += ptr_offset + 4;

    // Check if the timestamp of the frame is the same as the previously 
    // received one. The injection may be called multiple times for the same
    // received frame.
    // unsigned int time_stamp = (((*(frame_start+2))&0xffff0000) >> 16) | 
    //                           (((*(frame_start+3))&0x0000ffff) << 16);
    // unsigned long *timestamp_ptr = store_addr - 3;
    // if (*timestamp_ptr == time_stamp) {
    //     goto quit;
    // }
    // *timestamp_ptr = time_stamp;
    
    unsigned long *prev_index = (unsigned long *)BUFFER_ADDR;
    prev_index--;
    if (*prev_index == beam_index) {
        goto quit;
    }
    *prev_index = beam_index;
    
    // Update packet counter for the beam index.
    unsigned long *counter_ptr =  store_addr - 4;
    (*counter_ptr)++;
    
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
    *(target+SNR_ADDR_OFFSET) = (unsigned long)snr;
    
    // Advance the pointer of ring buffer. Rewind if necessary
    *head_ptr = *head_ptr + 1;
    if (*head_ptr - store_addr >= STORE_SIZE) {
        *head_ptr = store_addr;
    }
    
quit:
    // Restore the register values from the stack.
    asm("pop r6");
    asm("pop r5");
    asm("pop r4");
    asm("pop r3");
    asm("pop r2");
    asm("add sp, sp, 0x20");
}

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
    asm(".ascii \"Patched: per-beam-rss3 (ver: 3.1." BUILD_VER ")\n\""); 
}
