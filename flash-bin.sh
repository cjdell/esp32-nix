#!/bin/sh
set -Eeuo pipefail

idf.py build

#/nix/store/n5w5cvxmgwssin8sajxv1z5r08aal37z-python3-3.12.6-env/bin/python3.12 ../../../../nix/store/g3gip1zph7fq6s6q6sd3pmh3y1w2wgql-esp-idf-632e0c2a9fc7c754db4135dabb67f7fc6aa9fb87/components/esptool_py/esptool/esptool.py -b 460800 --before default_reset --after hard_reset --chip esp32s3  write_flash --flash_mode dio --flash_size 8MB --flash_freq 80m 0x0 build/bootloader/bootloader.bin 0x8000 build/partition_table/partition-table.bin 0x20000 build/esp32-s3-nix.bin
python -m esptool --chip esp32s3 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size 8MB --flash_freq 80m 0x0 build/bootloader/bootloader.bin 0x8000 build/partition_table/partition-table.bin 0x20000 build/esp32-s3-nix.bin
