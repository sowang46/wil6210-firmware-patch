// ROM:0001E0E4 loc_1E0E4:                              # CODE XREF: sub_1DED4+1BCj
// ROM:0001E0E4                 ld      r9, [r10,0x10]
// ROM:0001E0E8                 ld      r11, [sp,0x20+var_20]
// ROM:0001E0EC                 brge    r9, r11, loc_1E094
// ROM:0001E0F0                 mov     r0, 0x804010
// ROM:0001E0F8                 st      r11, [r10,0x10]
// ROM:0001E0FC                 ldw     r7, [r0,0x12]
// ROM:0001E100                 lsr     r7, r7, 0xA
// ROM:0001E104                 stb     r7, [r10,0xC]
// ROM:0001E108                 ldw     r8, [r0,0x14]
// ROM:0001E10C                 bmsk    r8, r8, 1
// ROM:0001E110                 b.d     loc_1E094
// ROM:0001E114                 stb     r8, [r10,0xD]


//   1e0e4:       10 12 09 10                     ld         r9,[r10,16]
//   1e0e8:       00 14 0b 30                     ld         r11,[sp,0]
//   1e0ec:       a9 09 c3 92                     brge       r9,r11,0x1e094

//   1e0f0:       0a 20 80 0f 80 00 10 40         mov        r0,0x00804010
//   1e0f8:       10 1a c0 12                     st         r11,[r10,16]
//   1e0fc:       12 10 07 01                     ldw        r7,[r0,18]
//   1e100:       41 2f 87 02                     lsr        r7,r7,10
//   1e104:       0c 1a c2 11                     stb        r7,[r10,12]
//   1e108:       14 10 08 01                     ldw        r8,[r0,20]
//   1e10c:       53 20 48 10                     bmsk       r8,r8,1
//   1e110:       85 07 ef ff                     b.d        0x1e094

//   1e114:       0d 1a 02 12                     stb        r8,[r10,13]


#pragma NEXMON targetregion "ucpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

void beam_rss() {
    // load beam index of current beacon frame
    asm("mov r0, 0x00804010");
    asm("ldw r7, [r0, 18]");
    asm("lsr r7, r7, 0xa");
    asm("asl r7, r7, 0x2");
    // update the counter
    asm("mov r8, 0x00942cc0");
    asm("add r8, r8, r7");
    asm("ld r0, [r8]");
    asm("add r0, r0, 1");
    asm("st r0, [r8]");
    // store the snr
    asm("mov r8, 0x00802de0");
    asm("add r8, r8, r7");
    asm("st r11, [r8]");
    // store the rss
    asm("mov r0, 0x00883BEC");
    asm("ld r8, [r0]");
    asm("mov r0, r8");
    asm("mov r8, 0x00802F00");
    asm("add r8, r8, r7");
    asm("st r0, [r8]");
    asm("mov r0, 0x00804010");
    // // store the cir into the upper area
    asm("ldw r7, [r0, 18]");
    asm("lsr r7, r7, 0xa");
    asm("asl r7, r7, 0x6");
    asm("mov r0, 0x00883844");
    asm("mov r8, 0x0093eb00");
    asm("add r8, r8, r7");
    asm("mov lp_count, 0x10");
    asm("lp 0xc");
    asm("ld.ab r7, [r0, 4]");
    asm("st.ab r7, [r8, 4]");
    asm("mov r0, 0x00804010");
    asm("ldw r7, [r0, 18]");
    asm("lsr r7, r7, 0xa");
    // resume normal -- rss small case
    asm("cmp r9, r11");
    asm("jge 0x1e094");
    // resume normal -- rss large case
    asm("st r11,[r10,16]"); // update the largest rss
    asm("stb r7,[r10,12]"); // update the best beam index
    asm("ldw r8,[r0,20]");
    asm("bmsk r8,r8,1");
    asm("stb r8,[r10,13]");
    asm("j 0x1e094");
}

__attribute__((at(UC_ADDR(0x1E0EC), "", CHIP_VER_WIL6210, FW_VER_620_40)))
BPatch(beam_rss, beam_rss);

