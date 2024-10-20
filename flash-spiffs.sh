#!/bin/sh

mkspiffs -c spiffs -s 0x400000 spiffs.bin

esptool.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x400000 spiffs.bin
