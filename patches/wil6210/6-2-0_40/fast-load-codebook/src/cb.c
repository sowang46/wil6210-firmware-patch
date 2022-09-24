#pragma NEXMON targetregion "fwpatch"

#include <firmware_version.h>
#include <patcher.h>
#include <wrapper.h>
#include <helper.h>

// ROM:0000D390                 cmp     r1, 1
// ROM:0000D392                 mov     r2, 1
// ROM:0000D394                 sub.nz  r2, r2, r2
// ROM:0000D396                 ld      r3, [r14,r13]
// ROM:0000D398                 add     r6, r14, r13
// ROM:0000D39C                 ld      r4, [r4,4]
// ROM:0000D3A0                 add     r7, r14, r13
// ROM:0000D3A4                 ld      r5, [r5,8]
// ROM:0000D3A8                 add     r1, r14, r13
// ROM:0000D3AA                 ld      r6, [r6,0xC]
// ROM:0000D3AE                 ld      r1, [r1,0x14]
// ROM:0000D3B0                 ld      r7, [r7,0x10]
// ROM:0000D3B4                 st      r1, [sp,0x10+var_10]
// ROM:0000D3B6                 mov     r1, 0xFF
// ROM:0000D3B8                 st      r1, [sp,0x10+var_C]
// ROM:0000D3BA                 bl.d    sub_14128
// ROM:0000D3BE                 ldw     r1, [r15]
// ROM:0000D3C0                 mov     r1, 0x10089EC
// ROM:0000D3C6                 ldb     r4, [r15,2]


    // d390:       81 e1                           cmp_s      r1,1
    // d392:       01 da                           mov_s      r2,1
    // d394:       c0 7a                           sub_s.ne   r2,r2,r2
    // d396:       a3 66                           ld_s       r3,[r14,r13]
    // d398:       00 26 46 13                     add        r6,r14,r13
    // d39c:       04 14 04 00                     ld         r4,[r4,4]
    // d3a0:       00 26 47 13                     add        r7,r14,r13
    // d3a4:       08 15 05 00                     ld         r5,[r5,8]
    // d3a8:       b9 66                           add_s      r1,r14,r13
    // d3aa:       0c 16 06 00                     ld         r6,[r6,12]
    // d3ae:       25 81                           ld_s       r1,[r1,20]
    // d3b0:       10 17 07 00                     ld         r7,[r7,16]
    // d3b4:       40 c1                           st_s       r1,[sp,0]
    // d3b6:       ff d9                           mov_s      r1,255
    // d3b8:       41 c1                           st_s       r1,[sp,4]
    
    // d3ba:       72 0d 60 03                     bl.d       0x14128

    // d3be:       20 97                           ldw_s      r1,[r15,0]
    // d3c0:       cf 71 00 01 ec 89               mov_s      r1,0x010089ec
    // d3c6:       02 17 84 10                     ldb        r4,[r15,2]

void hwd_rfc_write_sector(unsigned int, unsigned int, unsigned int, unsigned int,\
                          unsigned int, unsigned int, unsigned int, unsigned int,\
                          unsigned int, unsigned int);
void resume(void);

void fast_load_cb(int ignore, int cb_select) {
    char device_id;
    unsigned int sector_idx;
    unsigned int is_tx_sector;
    unsigned int psh_hi_val;
    unsigned int psh_lo_val;
    unsigned int etype0_val;
    unsigned int etype1_val;
    unsigned int etype2_val;
    unsigned int dtype_swch;
    unsigned int activate_sector;
    
    unsigned int *cb_addr;
    
    int sec_idx;
    for (sec_idx = 0; sec_idx < 64; sec_idx++) {
        cb_addr = (unsigned int *) (0x008fcb00 + (0x600+0x30)*cb_select + 4*6*sec_idx);
        
        device_id = 0;
        sector_idx = sec_idx;
        is_tx_sector = 1;
        psh_hi_val = *cb_addr;
        psh_lo_val = *(cb_addr+1);
        etype0_val = *(cb_addr+2);
        etype1_val = *(cb_addr+3);
        etype2_val = *(cb_addr+4);
        dtype_swch = *(cb_addr+5);
        activate_sector = 0xff;
        
        hwd_rfc_write_sector(device_id, sector_idx, is_tx_sector, psh_hi_val,\
                             psh_lo_val, etype0_val, etype1_val, etype2_val, \
                             dtype_swch, activate_sector);
    }
    
    resume();
}

