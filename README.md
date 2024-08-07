# Raspberry PI Pico W 6502 microprocessor ROM emulator

This is a simple ROM emulator for the 6502 microprocessor using the Raspberry PI Pico. The emulator can be used to test the 6502 microprocessor without the need for a physical ROM/EEPROM chip. The emulator can be used to test the 6502 microprocessor with a clock frequency of up to 1kHz.

The entire 6502 build and examples is inspired by [Ben Eater's 6502 computer build](https://eater.net/6502)

## Requirements
- ARM toolchain
- GCC
- CMake
- Python
- Raspberry PI Pico SDK
- VASM Old-style assembler [Download Here](http://www.compilers.de/vasm.html)
- XXD (For converting binary to C header file)

## GPIO Assignments

- PIN 0-15: Address Bus (A0-A15)
- PIN 16-22, 26: Data Bus (D7-D0)
- PIN 27: Read/Write (R/W)
- PIN 28: Clock (From the clock/timer)

## Generating the ROM

### Using the `makerom_py.sh` script
There are two ways to generate a ROM one is using the `makerom.py` this generates a test ROM file with a simple instructions. The instructions
are coded directly on the script under the `code` variable and it contains the hex equivalent of the instructions.

### Using the `vasm` assembler `makerom_asm.sh <assembly_file>`
The second way is to use the `vasm` assembler to generate the ROM. The `makerom_asm.sh` script assembles the given assembly file and generates a binary file.
The binary file is then converted to a C header file using the `xxd` command. Make sure to add the `.section code, "rx"` directive to the start of your assembly files to make sure that the code is placed in the correct section of the binary file.

## Building and Flashing
```bash
[makerom_py.sh | makerom_asm.sh <assembly_file>] && ./upload.sh
```

Make sure that the Pico is in BOOTSEL mode before running the script.

## Working with RAM (6502 Stack R/W)
To avoid conflicts with an external RAM a `ROM_STACK_START=0x0000` and `ROM_STACK_END=0x07FF` is defined in the `CMakeLists.txt` file. The ROM will set the data pins to high impedance when the address is within this stack range to avoid bus contention.