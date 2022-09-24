#pragma NEXMON targetregion "fwpatch"

#include <firmware_version.h>
#include <patcher.h>
#include "ver.h"

#define BUFFER_ADDR 0x8FE004
#define STACK_ADDR "0x8FE000"
#define STORE_REG(r, i) \
    asm("st " #r ", [" STACK_ADDR "-0x4*" #i "]");
#define LOAD_REG(r, i) \
    asm("ld " #r ", [" STACK_ADDR "-0x4*" #i "]");

unsigned int post_pending_event_11u_scheduler(unsigned int, unsigned int, 
                                    unsigned int, unsigned int, 
                                    unsigned int, unsigned int, 
                                    unsigned int);
void hwd_rfc_rx_sectgain_on(unsigned int, unsigned int, unsigned int);
void hwd_rfc_tx_sectgain_on(unsigned int, unsigned int, unsigned int);

void periodic_beam_switch_handler();
void schedule_beam_switch_timer();

void periodic_beam_switch_handler() {
    STORE_REG(r0, 20)
    STORE_REG(r1, 21)
    STORE_REG(r2, 22)
    STORE_REG(r3, 23)
    STORE_REG(r4, 24)
    unsigned int notcare = 0;
    unsigned int rf_sector = 0;
    unsigned int rf_gain_idx = 0;
    
    unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
    unsigned long *periodic_ret = store_addr+0x74/4;
    
    unsigned long *counter = store_addr+2;
    (*counter)++;
    
    unsigned long *is_tx_mode = store_addr+3;
    unsigned long *curr_beam = store_addr+4;
    unsigned long *max_beam = store_addr+5;
    unsigned long *gain_idx = store_addr+6;
    
    rf_sector = *curr_beam;
    rf_gain_idx = *gain_idx;
    
    if (*is_tx_mode) {
        hwd_rfc_tx_sectgain_on(notcare, rf_sector, rf_gain_idx);
    } else {
        hwd_rfc_rx_sectgain_on(notcare, rf_sector, rf_gain_idx);
    }
    
    (*curr_beam)++;
    if ((*curr_beam) > (*max_beam)) {
        *curr_beam = 0;
    }
    
    *periodic_ret = 0;
    schedule_beam_switch_timer();
    
    LOAD_REG(r0, 20)
    LOAD_REG(r1, 21)
    LOAD_REG(r2, 22)
    LOAD_REG(r3, 23)
    LOAD_REG(r4, 24)
}

void schedule_beam_switch_timer() {
    STORE_REG(r0, 10)
    STORE_REG(r1, 11)
    STORE_REG(r2, 12)
    STORE_REG(r3, 13)
    STORE_REG(r4, 14)
    STORE_REG(r5, 15)
    STORE_REG(r6, 16)
    
    unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
    unsigned long *periodic_run = store_addr+0x78/4;
    unsigned long *periodic_val = store_addr+0x70/4;
    unsigned long *periodic_ret = store_addr+0x74/4;
    
    unsigned int handler_func_addr = ((unsigned int)(&periodic_beam_switch_handler)-0x8c0000);
    unsigned int data_store_addr = BUFFER_ADDR;
    unsigned int data_r2 = 0;
    unsigned int data_r3 = 0;
    unsigned int periodicity = (*periodic_val);
    unsigned int data_r5 = 0;
    unsigned int data_r6 = 0;
    
    if ((*periodic_run) && !(*periodic_ret) && (*periodic_val)) {
        unsigned int ret = post_pending_event_11u_scheduler(handler_func_addr, 
                            data_store_addr, data_r2, data_r3, periodicity, 
                            data_r5, data_r6);
        *periodic_ret = ret;
    }
    
    LOAD_REG(r0, 10)
    LOAD_REG(r1, 11)
    LOAD_REG(r2, 12)
    LOAD_REG(r3, 13)
    LOAD_REG(r4, 14)
    LOAD_REG(r5, 15)
    LOAD_REG(r6, 16)
}

void echo_hook(int echo_val) {
    STORE_REG(r0, 0)
    STORE_REG(r2, 2)
    STORE_REG(r3, 3)
    
    asm("mov_s r0, r13");
    
    if (echo_val == 0x1100) {
        unsigned long *store_addr = (unsigned long *)BUFFER_ADDR;
        unsigned long *periodic_run = store_addr+0x78/4;
        unsigned long *periodic_val = store_addr+0x70/4;
        unsigned long *periodic_ret = store_addr+0x74/4;
        unsigned long *counter = store_addr+1;
        (*counter)++;
        
        if ((*periodic_val) == 0) {
            *periodic_val = 100000;
        }
        *periodic_ret = 0;
        if (!(*periodic_run)) {
            *periodic_run = 1;
            schedule_beam_switch_timer();
            asm("mov r13, 0x2000");
        } else {
            asm("mov r13, 0x2100");
        }
    }
    
    LOAD_REG(r0, 0)
    LOAD_REG(r2, 2)
    LOAD_REG(r3, 3)
    asm("pop_s blink");
    
    asm("st_s r13,[r0,0]");
    asm("mov_s r2, 0");
    asm("j 0x25bc8");
}

// Patch version information
__attribute__((at(UC_ADDR(0x1ff00), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked))
void str_ver(void) { 
    asm(".ascii \"Patched: periodic-beam-switch (ver: 1.0." BUILD_VER ")\"");
}