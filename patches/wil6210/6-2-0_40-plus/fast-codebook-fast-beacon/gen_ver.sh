#!/bin/bash
BUILD_VER=$(($(cat src/ver.h | grep BUILD_VER | awk '{print $3}' | sed 's/\"//g')+1));

echo -e "#ifndef __VER_H__\n"\
"#define __VER_H__\n\n"\
"#define BUILD_VER \"$BUILD_VER\"\n\n"\
"#endif /* __VER_H__ */"\
> ./src/ver.h