# Example ESP-IDP Project Nix Flake

Example ESP32-S3 project which flashes an LED.

This is mostly a learning exercise to help me master the Nix programming language.

## Building and developing

    nix develop

    idf.py build

    idf.py flash

### VSCode Intellisense

You must have the `direnv` package installed on the host and have the recommended VSCode extensions installed. The CMake extension won't detect the compiler correctly until the project is built with `idf.py build` and until the `.envrc` file is "allowed" by `direnv`.
