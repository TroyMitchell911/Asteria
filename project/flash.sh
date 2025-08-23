#/bin/bash

cd build_sf32lb52-lchspi-ulp_hcpu/

sftool -p "$1" -c SF32LB52 write_flash "bootloader/bootloader.bin@0x12010000" "main.bin@0x12020000" "ftab/ftab.bin@0x12000000"

