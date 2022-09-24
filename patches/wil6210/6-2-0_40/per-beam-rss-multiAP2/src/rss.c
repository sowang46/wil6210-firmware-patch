#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>
#include "ver.h"

#define STORE_SIZE 5
#define BEAM_NUM 64
#define MAX_AP_NUM 3
#define PAGE_SIZE BEAM_NUM * (STORE_SIZE + 4)

#define BUFFER_ADDR 0x1eb00
#define STACK_ADDR "0x1eae0"
#define MAC_LIST_ADDR 0x1eb00

#define STORE_REG(r, i) \
    asm("st " #r ", [" STACK_ADDR "-0x4*" #i "]");
#define LOAD_REG(r, i) \
    asm("ld " #r ", [" STACK_ADDR "-0x4*" #i "]");
    
// #define AP1_MAC_ADDR 0x96
// #define AP2_MAC_ADDR 0x15

void beacon_rss_common() {
    // Push registers in use to stack.
    // Make sure this patch won't modify those register values.
    STORE_REG(blink, 2)
    STORE_REG(r2, 3)
    STORE_REG(r3, 4)
    STORE_REG(r4, 5)
    STORE_REG(r5, 6)
    STORE_REG(r6, 7)
    
    // Check frame type. Make sure it is beacon frame.
    unsigned int *frame_start = (unsigned int *)0x00804010;
    unsigned int frame_type = (*frame_start) & 0xffff;
    if (frame_type != 0x000c) {
        goto quit;
    }
    
    // Get the MAC address -- which AP sends the beacon frame.
    unsigned char *mac_addr = (unsigned char *)(frame_start+1);
    unsigned char *mac_list_ptr = (unsigned char *)MAC_LIST_ADDR;
    unsigned int page_index = 0;
    for (page_index = 0; page_index < MAX_AP_NUM; page_index++) {
        if (*(mac_list_ptr-page_index-1) && \
            *(mac_list_ptr-page_index-1) == mac_addr[5]) {
            break;
        }
    }
    
    // The beacon is sent by AP which is not in the list
    if (page_index == MAX_AP_NUM) {
        goto quit;
    }
    
    // Get the beam index.
    unsigned int beam_index = *(frame_start+4);
    beam_index >>= 0x1a;
    
    // Calculate the memory storage position for the beam index.
    unsigned int ptr_offset = page_index * PAGE_SIZE + \
                              beam_index * (STORE_SIZE + 4);
    unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
    store_addr += ptr_offset + 3;
    
    // Check if the timestamp of the frame is the same as the previously 
    // received one. The injection may be called multiple times for the same
    // received frame.
    unsigned int time_stamp = (((*(frame_start+2)) & 0xffff0000) >> 16) | 
                              (((*(frame_start+3)) & 0x0000ffff) << 16);
    unsigned long *timestamp_ptr = store_addr - 2;
    if (*timestamp_ptr == time_stamp) {
        goto quit;
    } else {
        *timestamp_ptr = time_stamp;
    }
    
    // Update packet counter for the beam index.
    unsigned short *counter_mac_ptr = (unsigned short *)(store_addr - 3);
    (*counter_mac_ptr)++;
    
    // Store the mac address
    *(counter_mac_ptr+1) = (*((unsigned long *)(mac_addr+4))) & 0xFFFF;
    
    // Head pointer of the ring buffer.
    unsigned short **head_ptr =  (unsigned short **)(store_addr - 1);
    if (*head_ptr == 0) {
        *head_ptr = (unsigned short *)store_addr;
    }
    
    // Write RSS to the head
    unsigned short *target = *head_ptr;
    unsigned long *rss = (unsigned long *)0x00883BEC;

    *target = (((*rss) & 0xFFFFF) >> 8) & 0xFFFF;
    
    // Advance the pointer of ring buffer. Rewind if necessary
    *head_ptr = *head_ptr + 1;
    if (*head_ptr - (unsigned short *)store_addr >= STORE_SIZE) {
        *head_ptr = (unsigned short *)store_addr;
    }
    
quit:
    // Restore the register values from the stack.
    LOAD_REG(blink, 2)
    LOAD_REG(r2, 3)
    LOAD_REG(r3, 4)
    LOAD_REG(r4, 5)
    LOAD_REG(r5, 6)
    LOAD_REG(r6, 7)
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
__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_410_55)))
__attribute__((naked))
void str_ver(void) { 
    asm(".ascii \"Patched: per-beam-rss-multiAP2 (ver: 5.2." BUILD_VER ")\n\""); 
}
