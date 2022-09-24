#pragma NEXMON targetregion "ucpatch"
#include "util.h"
#include "log.h"

LOG_HANDLER(L_97c, push_s blink\njl 0xcc08\n)
PATCH_LOG_N6(L_97c)