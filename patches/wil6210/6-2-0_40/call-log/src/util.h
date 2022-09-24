#ifndef UTIL_H
#define UTIL_H

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

#define NOP_0
#define NOP_2 asm("nop_s");
#define NOP_4 asm("nop");
#define NOP_6 NOP_2 NOP_4

#define LOG_HANDLER(addr, val) \
void r_ ## addr(); \
__attribute__((naked)) void log_ ## addr(void) { \
    asm("push r0"); \
    asm("push_s blink"); \
    log_it(addr); \
    asm("pop_s blink"); \
    asm("pop r0"); \
    \
    asm(#val);\
    r_ ## addr(); \
}

#define PATCH_LOG_N4(addr) \
__attribute__((at(UC_ADDR(addr), "", CHIP_VER_WIL6210, FW_VER_620_40))) \
__attribute__((naked)) void b_ ## addr(void) { \
    NOP_0 \
    log_ ## addr(); \
}

#define PATCH_LOG_N6(addr) \
__attribute__((at(UC_ADDR(addr), "", CHIP_VER_WIL6210, FW_VER_620_40))) \
__attribute__((naked)) void b_ ## addr(void) { \
    NOP_2 \
    log_ ## addr(); \
}

#define PATCH_LOG_N8(addr) \
__attribute__((at(UC_ADDR(addr), "", CHIP_VER_WIL6210, FW_VER_620_40))) \
__attribute__((naked)) void b_ ## addr(void) { \
    NOP_4 \
    log_ ## addr(); \
}

#define PATCH_LOG_N10(addr) \
__attribute__((at(UC_ADDR(addr), "", CHIP_VER_WIL6210, FW_VER_620_40))) \
__attribute__((naked)) void b_ ## addr(void) { \
    NOP_6 \
    log_ ## addr(); \
}

#define RESUME_10(addr) \
__attribute__((at(UC_ADDR(addr+10), "dummy", CHIP_VER_WIL6210, FW_VER_620_40))) \
void r_ ## addr(void) { ; }

#define RESUME_8(addr) \
__attribute__((at(UC_ADDR(addr+8), "dummy", CHIP_VER_WIL6210, FW_VER_620_40))) \
void r_ ## addr(void) { ; }

#define RESUME_6(addr) \
__attribute__((at(UC_ADDR(addr+6), "dummy", CHIP_VER_WIL6210, FW_VER_620_40))) \
void r_ ## addr(void) { ; }

#define RESUME_4(addr) \
__attribute__((at(UC_ADDR(addr+4), "dummy", CHIP_VER_WIL6210, FW_VER_620_40))) \
void r_ ## addr(void) { ; }


void log_it(int index);

#endif