#!/bin/sh

mkspiffs -c spiffs -s 0x400000 spiffs.bin

esptool.py --baud 921600 write_flash 0x400000 spiffs.bin
