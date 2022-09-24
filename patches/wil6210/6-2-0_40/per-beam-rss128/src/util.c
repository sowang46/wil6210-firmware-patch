#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>


// __attribute__((naked)) void counter_1(void) {
//     asm("push r2");
//     asm("push r3");
    
//     unsigned int *addr = 0x00942dc0;
//     (*addr)++;
    
//     asm("pop r3");
//     asm("pop r2");
    
//     asm("ld         blink,[sp,4]");
    
//     resume_1();
// }

// __attribute__((naked)) void counter_2(void) {
//     asm("push r2");
//     asm("push r3");
    
//     unsigned int *addr = 0x00942dc4;
//     (*addr)++;
    
//     asm("pop r3");
//     asm("pop r2");
    
//     asm("mov_s      r14,0x00804010");
    
//     resume_2();
// }

// __attribute__((naked)) void counter_3(void) {
//     asm("push r2");
//     asm("push r3");
    
//     unsigned int *addr = 0x00942dc8;
//     (*addr)++;
    
//     asm("pop r3");
//     asm("pop r2");
    
//     asm("jl         0x1682c");
    
//     resume_3();
// }

// __attribute__((naked)) void counter_4(void) {
//     asm("push r2");
//     asm("push r3");
    
//     unsigned int *addr = 0x00942dcc;
//     (*addr)++;
    
//     asm("pop r3");
//     asm("pop r2");
    
//     asm("st         blink,[sp,4]");
    
//     resume_4();
// }

// __attribute__((naked)) void counter_5(void) {
    
//     asm("push r2");
//     asm("push r3");
    
//     unsigned int *addr = 0x00942dd0;
//     (*addr)++;
    
//     asm("pop r3");
//     asm("pop r2");
    
//     // asm("push r2");
//     // asm("push r3");
//     // asm("mov_s r2,0x00942dcc");
//     // asm("ld_s r3,[r2,0]");
//     // asm("add_s r3,r3,1");
//     // asm("st_s r3,[r2,0]");
//     // asm("pop r3");
//     // asm("pop r2");
    
//     // asm("mov        r1,0");
    
//     resume_5();
// }

// __attribute__((naked)) void test(void) {
//     asm("push r2");
//     asm("mov r2, 0x00942dd0");
//     asm("st r0, [r2]");
//     asm("pop r2");
//     //
//     asm("st         blink,[sp,4]");
    
//     asm("j 0x3990");
// }