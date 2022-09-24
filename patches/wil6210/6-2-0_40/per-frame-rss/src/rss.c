#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>
#include "ver.h"

#define BUFFER_ADDR 0x0093eb00
#define STACK_ADDR "0x1eae0"
#define STORE_REG(r, i) \
    asm("st " #r ", [" STACK_ADDR "-0x4*" #i "]");
#define LOAD_REG(r, i) \
    asm("ld " #r ", [" STACK_ADDR "-0x4*" #i "]");

void
beacon_rss_common() {
    // Push registers in use to stack.
    // Make sure this patch won't modify those register values.
    STORE_REG(r2, 3)
    STORE_REG(r3, 4)
    STORE_REG(r4, 5)
    STORE_REG(r5, 6)
    
    // Check frame type and beam index of beacon frame.
    unsigned int *frame_start = (unsigned int *)0x00804010;
    unsigned int frame_type = (*frame_start)&0xffff;
    if (frame_type != 0x000c) {
        goto quit;
    }
    
    unsigned int beam_index = *(frame_start+4);
    beam_index >>= 0x1a;
    if (beam_index != 0x0000) {
        goto quit;
    }
    
    // Get the MAC address -- which AP sends the beacon frame.
    unsigned char *mac_addr = (unsigned char *)(frame_start+1);
    if (mac_addr[5] != 0x15) {
        goto quit;
    }
    
    // Check if the timestamp of the frame is the same as the previously 
    // received one. The injection may be called multiple times for the same
    // received frame.
    unsigned int time_stamp = (((*(frame_start+2))&0xffff0000) >> 16) | 
                              (((*(frame_start+3))&0x0000ffff) << 16);
    unsigned int *timestamp_ptr = (unsigned int *)(BUFFER_ADDR - 16);
    if (*timestamp_ptr == time_stamp) {
        goto quit;
    } else {
        *timestamp_ptr = time_stamp;
    }
    
    unsigned int *rss = (unsigned int *)0x00883BEC;
    unsigned int *head_ptr = (unsigned int *)(BUFFER_ADDR - 8);
    unsigned int *tail_ptr = (unsigned int *)(BUFFER_ADDR - 4);
    unsigned int *target;
    
    if (*head_ptr == 0 || *tail_ptr == 0) {
        *head_ptr = BUFFER_ADDR;
        *tail_ptr = BUFFER_ADDR;
    }
    
    if (*rss < 700000) {
        if ((*head_ptr+4-BUFFER_ADDR) % 4096 != *tail_ptr-BUFFER_ADDR) {
            target = (unsigned int *)(*head_ptr);
            *target = *rss;
            *head_ptr = BUFFER_ADDR + (*head_ptr+4-BUFFER_ADDR) % 4096;
        } else {
            // ring buffer overflows
            target = (unsigned int *)(BUFFER_ADDR-0xc);
            *target = 1;
        }
    }
    
quit:
    LOAD_REG(r2, 3)
    LOAD_REG(r3, 4)
    LOAD_REG(r4, 5)
    LOAD_REG(r5, 6)
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

// Patch version information
__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked))
void str_ver(void) { 
    asm(".ascii \"Patched: per-frame-rss (ver: 1.1." BUILD_VER ")\n\""); 
}
