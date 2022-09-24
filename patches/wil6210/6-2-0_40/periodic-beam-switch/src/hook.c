#pragma NEXMON targetregion "fwpatch"

#include <firmware_version.h>
#include <patcher.h>
#include "ver.h"

void echo_hook();
__attribute__((at(FW_ADDR(0x25bc4), "", CHIP_VER_WIL6210, FW_VER_620_40)))
__attribute__((naked)) void _echo_hook(void) {
    echo_hook();
}

__attribute__((at(FW_ADDR(0x2143c), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
void post_pending_event_11u_scheduler(void) { ; }

__attribute__((at(FW_ADDR(0x1373c), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
void hwd_rfc_rx_sectgain_on(void) { ; }

__attribute__((at(FW_ADDR(0x13f0c), "dummy", CHIP_VER_WIL6210, FW_VER_620_40)))
void hwd_rfc_tx_sectgain_on(void) { ; }
