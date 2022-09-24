rm dump -rf
~/codebase/build-bin/wixnet/wil6210/fw_check/wil6210_fw_check --dump=true
arc-elf32-objdump -D -b binary -m arc600 -EL dump/dump_0x00920000.bin > dump/dump_0x00920000.asm

# arc-elf32-objdump -D -b binary -m arc600 -EL $1 > /tmp/dump.txt
# START_LINE=$(cat /tmp/dump.txt | grep -n $2 | awk '{print $1}' | sed 's/://')
# echo $START_LINE
# head /tmp/dump.txt -n $(($START_LINE+20)) | tail -n 30