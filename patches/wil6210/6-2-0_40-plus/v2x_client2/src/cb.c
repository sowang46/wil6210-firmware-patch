#pragma NEXMON targetregion "fwpatch"

#include <firmware_version.h>
#include <patcher.h>
#include "ver.h"

#define BUFFER_ADDR 0x008fe000
#define IN_PARAMS_ADDR 0x008fe000-4
#define BEAM_NUM 128

void hwd_rfc_write_sector(unsigned int, unsigned int, unsigned int, unsigned int,\
                          unsigned int, unsigned int, unsigned int, unsigned int,\
                          unsigned int, unsigned int);

void hwd_abif_dvs_rfca_cmd_enable(unsigned int, unsigned int *);
                          
void resume(void);

// d608:       0c 16 06 00                     ld         r6,[r6,12]
// d60c:       25 81                           ld_s       r1,[r1,20]
// d60e:       10 17 07 00                     ld         r7,[r7,16]
// d612:       40 c1                           st_s       r1,[sp,0]
// d614:       ff d9                           mov_s      r1,255
// d616:       41 c1                           st_s       r1,[sp,4]

// d618:       12 08 e0 03                     bl.d       0x14e28
// d61c:       20 97                           ldw_s      r1,[r15,0]

// d61e:       cf 71 00 01 fc 8c               mov_s      r1,0x01008cfc
// d624:       02 17 84 10                     ldb        r4,[r15,2]


void fast_load_cb() {
    unsigned int *in_params = (unsigned int *) (IN_PARAMS_ADDR);
    
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
    
    unsigned int module_vec;
    unsigned int *unknown_ptr = 0;
    
    int sec_idx;
    for (sec_idx = 0; sec_idx < BEAM_NUM; sec_idx++) {
        module_vec = *in_params;
        
        cb_addr = (unsigned int *) (BUFFER_ADDR + 4*6*sec_idx);
        
        device_id = 0;
        sector_idx = sec_idx;
        is_tx_sector = 0;
        psh_hi_val = *cb_addr;
        psh_lo_val = *(cb_addr+1);
        etype0_val = *(cb_addr+2);
        etype1_val = *(cb_addr+3);
        etype2_val = *(cb_addr+4);
        dtype_swch = *(cb_addr+5);
        if ((etype0_val|etype1_val|etype2_val) == 0) {
            activate_sector = 0;
        } else {
            activate_sector = 2;
        }

        hwd_abif_dvs_rfca_cmd_enable(module_vec, unknown_ptr);
        
        hwd_rfc_write_sector(device_id, sector_idx, is_tx_sector, psh_hi_val,\
                             psh_lo_val, etype0_val, etype1_val, etype2_val, \
                             dtype_swch, activate_sector);
    }
    
    resume();
}
