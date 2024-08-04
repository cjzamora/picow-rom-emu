# Raspberry PI Pico W 6502 microprocessor ROM emulator

This is a simple ROM emulator for the 6502 microprocessor using the Raspberry PI Pico. The emulator can be used to test the 6502 microprocessor without the need for a physical ROM/EEPROM chip. The emulator can be used to test the 6502 microprocessor with a clock frequency of up to 1kHz.

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

### Using the `vasm` assembler `makerom_asm.sh`
The second way is to use the `vasm` assembler to generate the ROM. The `makerom_asm.sh` script assembles the `src/rom.s` file and generates a binary file.
The binary file is then converted to a C header file using the `xxd` command. When customizing the `src/rom.s` file make sure to add the `.section code, "rx"` directive to the start of the file to make sure that the code is placed in the correct section of the binary file.

## Building and Flashing
```bash
[makerom_py.sh | makerom_asm.sh] && ./upload.sh
```

Make sure that the Pico is in BOOTSEL mode before running the script.