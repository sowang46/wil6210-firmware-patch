#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

void log_it(int index) {
    asm("push r2");
    asm("push r3");
    asm("push r4");
    
    unsigned int *head_ptr = (unsigned int *)(0x0093eb00-0x8);
    unsigned int *tail_ptr = (unsigned int *)(0x0093eb04-0x8);
    unsigned int *target;
    
    if (*head_ptr == 0 || *tail_ptr == 0) {
        *head_ptr = 0x0093eb00;
        *tail_ptr = 0x0093eb00;
    }
    
    if ((*head_ptr+4-0x0093eb00) % 4096 != *tail_ptr-0x0093eb00) {
        target = (unsigned int *)(*head_ptr);
        *target = index;
        *head_ptr = 0x0093eb00 + (*head_ptr+4-0x0093eb00) % 4096;
    } else {
        // ring buffer overflows
        target = (unsigned int *)(0x0093eb00-0xc);
        *target = 1;
    }
    
    asm("pop r4");
    asm("pop r3");
    asm("pop r2");
}