#include <stdio.h>
#include "pico/stdlib.h"
#include "bin.h"
#include "rom.h"

/**
 * Clock pin
 * 
 * @var int
 */
const int CLOCK_PIN = 28;

/**
 * Address pins
 * 
 * A0 - PIO 0 (MSB) - A15 - PIO 15 (LSB)
 * 
 * @var int[]
 */
const int ADDRESS_PINS[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

/**
 * Data pins
 * 
 * D0 - PIO 16 (MSB) - D7 - PIO 26 (LSB)
 * 
 * @var int[]
 */
const int DATA_PINS[] = { 16, 17, 18, 19, 20, 21, 22, 26 };

/**
 * Read write pin
 * 
 * @var int
 */
const int READ_WRITE_PIN = 27;

#ifndef ROM_STACK_START
#define ROM_STACK_START 0x0000
#endif

#ifndef ROM_STACK_END
#define ROM_STACK_END 0x07FF
#endif

/**
 * Rom stack start
 * 
 * @var u_int16_t
 */
const u_int16_t STACK_START = ROM_STACK_START;

/**
 * Rom stack end
 * 
 * @var u_int16_t
 */
const u_int16_t STACK_END = ROM_STACK_END;

/**
 * Read address sending by 6502 from address pins
 * 
 * @return u_int16_t
 */
u_int16_t rom_read_address() 
{
    u_int16_t address = 0;
    for (int i = 17; i >= 0; i--) {
        int bit = gpio_get(ADDRESS_PINS[i]) ? 1 : 0;
        address = (address << 1) + bit;
    }

    return address;
}

/**
 * Read data from data pins
 * 
 * @return u_int8_t
 */
u_int8_t rom_read_data() 
{
    u_int8_t data = 0;
    for (int i = 0; i < 8; i++) {
        int bit = gpio_get(DATA_PINS[i]) ? 1 : 0;
        data = (data << 1) + bit;
    }

    return data;
}

/**
 * Read address sending by 6502 in binary
 * 
 * @param u_int16_t address
 * @return char *
 */
char *rom_read_address_bin(u_int16_t address) 
{
    // convert address to binary
    static char bin[17];
    for (int i = 0; i < 16; i++) {
        bin[i] = (address >> (15 - i)) & 1 ? '1' : '0';
    }

    bin[16] = '\0';

    return bin;
}

/**
 * Read data from rom in binary
 * 
 * @param u_int8_t data
 * @return char *
 */
char *rom_read_data_bin(u_int8_t data)
{
    // convert data to binary
    static char bin[9];
    for (int i = 0; i < 8; i++) {
        bin[i] = (data >> (7 - i)) & 1 ? '1' : '0';
    }

    bin[8] = '\0';

    return bin;
}

/**
 * Check if 6502 is reading
 * 
 * @return bool
 */
bool rom_is_read_req()
{
    return gpio_get(READ_WRITE_PIN);
}

/**
 * Write data to data pins to be read by 6502
 * 
 * @param u_int8_t data
 * @return void
 */
void rom_write_data(u_int8_t data) 
{
    // data should be written in reverse order
    // because PIO 16 is MSB and PIO 26 is LSB
    for (int i = 0; i < 8; i++) {
        int bit = (data >> (7 - i)) & 1;
        gpio_put(DATA_PINS[i], bit);
    }

    return;
}

/**
 * Set data pins to output mode
 * 
 * @return void
 */
void rom_data_dir_out()
{
    for (int i = 0; i < 8; i++) {
        gpio_set_dir(DATA_PINS[i], GPIO_OUT);
    }
}

/**
 * Set data pins to input mode
 * 
 * @param bool clr_pins
 * @return void
 */
void rom_data_dir_in(bool clr_pins)
{
    for (int i = 0; i < 8; i++) {
        if (clr_pins) {
            gpio_put(DATA_PINS[i], 0);
        }

        gpio_set_dir(DATA_PINS[i], GPIO_IN);
    }
}

/**
 * Rom clock interrupt handler
 * 
 * @param uint gpio
 * @param uint32_t events
 * @return void
 */
void rom_clock_irq(uint gpio, uint32_t events) 
{
    // read address and data from 6502
    u_int16_t address = rom_read_address();
    // determine if 6502 is reading or writting
    char mode = rom_is_read_req() ? 'r' : 'W';
    // avoid conflicts with RAM check if address is within the stack range
    bool is_stack = address >= STACK_START && address <= STACK_END;

    // - if 6502 is writting, set the data pins to high impedance
    //   to prevent pico and 6502 to write data at the same time (Bus Contention).
    // - if if address is within the stack range, set the data pins 
    //   to high impedance to avoid pico and RAM to write data at the same time.
    if (mode == 'W' || is_stack) {
        rom_data_dir_in(false);
    } else {
        rom_data_dir_out();

        // read data from rom
        u_int8_t data = __rom__[address];
        // write data to 6502
        rom_write_data(data);
    }
}


/**
 * Initialize rom
 * 
 * @return void
 */
void rom_init() 
{
    // initialize clock pin
    gpio_init(CLOCK_PIN);
    // set clock pin to input mode
    gpio_set_dir(CLOCK_PIN, GPIO_IN);
    // add clock interrupt
    gpio_set_irq_enabled_with_callback(CLOCK_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &rom_clock_irq);

    // initialize address pins
    for (int i = 0; i < 16; i++) {
        gpio_init(ADDRESS_PINS[i]);
        gpio_set_dir(ADDRESS_PINS[i], GPIO_IN);
        gpio_pull_down(ADDRESS_PINS[i]);
    }

    // initialize data pins
    for (int i = 0; i < 8; i++) {
        gpio_init(DATA_PINS[i]);
        gpio_set_dir(DATA_PINS[i], GPIO_OUT);
        gpio_pull_down(DATA_PINS[i]);
    }

    // initialize read write pin
    gpio_init(READ_WRITE_PIN);
    return;
}