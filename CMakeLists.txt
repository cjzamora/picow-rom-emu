cmake_minimum_required(VERSION 3.13)

# set project name
set(PROJECT picow_rom_emu)
# set pico board
set(PICO_BOARD pico_w)

# initialize the SDK based on PICO_SDK_PATH
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# set the project name
project(${PROJECT} C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# add the executable
add_executable(${PROJECT} 
    src/main.c
    src/rom.c
    src/rom_bin.h
)

# add compile definitions
add_compile_definitions(
    ROM_STACK_START=0x0000
    ROM_STACK_END=0x07FF
)

# add target link libraries
target_link_libraries(
    ${PROJECT}
    pico_stdlib
    pico_cyw43_arch_none
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT})
# enable USB output
pico_enable_stdio_usb(${PROJECT} 1)
# enable UART output
pico_enable_stdio_uart(${PROJECT} 1)