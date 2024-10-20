# Example ESP-IDP Project Nix Flake

Example ESP32-S3 project which flashes an LED.

This is mostly a learning exercise to help me master the Nix programming language.

## Building and developing

    nix develop

    idf.py build

    idf.py flash

### VSCode Intellisense

You must have the `direnv` package installed on the host and have the recommended VSCode extensions installed. The CMake extension won't detect the compiler correctly until the project is built with `idf.py build` and until the `.envrc` file is "allowed" by `direnv`.

## References

https://github.com/Mjrovai/XIAO-ESP32S3-Sense
https://github.com/espressif/arduino-esp32/blob/master/variants/XIAO_ESP32S3/pins_arduino.h

https://github.com/Seeed-Studio/wiki-documents/issues/719
E (407) quad_psram: PSRAM ID read error: 0x00ffffff, PSRAM chip not found or not supported, or wrong PSRAM line mode
