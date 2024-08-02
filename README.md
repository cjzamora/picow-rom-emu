# Raspberry PI Pico W 6502 microprocessor ROM emulator

This is a simple ROM emulator for the 6502 microprocessor using the Raspberry PI Pico. The emulator can be used to test the 6502 microprocessor without the need for a physical ROM/EEPROM chip. The emulator can be used to test the 6502 microprocessor with a clock frequency of up to 1MHz.

## Requirements
- ARM toolchain
- GCC
- CMake
- Python
- Raspberry PI Pico SDK

## GPIO Assignments

- PIN 0-15: Address Bus (A0-A15)
- PIN 16-22, 26: Data Bus (D7-D0)
- PIN 27: Read/Write (R/W)
- PIN 28: Clock (From the clock/timer)

## About makerom.py
The makerom.py script is used to generate the ROM binary file and then converted to a C header file so that the ROM data can be included in the source code. This is not the most efficient way to do this but for testing purposes, it is good enough. You can modify the code part of the script depending on your 6502 instructions.

The process of generating the ROM can also be changed by writting an actual 6502 assembly code then compiling it into machine code, then converting it into a C header file so that you don't have to put the instruction hex codes manually on the makerom.py script.

## Building and Flashing
```bash
./upload.sh
```

Make sure that the Pico is in BOOTSEL mode before running the script.