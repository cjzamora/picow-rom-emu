#include <stdio.h>
#include "pico/stdlib.h"

// define address pins
// A0 - PIO 0 (MSB) - A15 - PIO 15 (LSB)
const int ADDRESS_PINS[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
// define data pins
// D0 - PIO 16 (MSB) - D7 - PIO 26 (LSB)
const int DATA_PINS[] = { 16, 17, 18, 19, 20, 21, 22, 26 };
// define read write pin
const int READ_WRITE_PIN = 27;

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
        // switch data pins to input mode
        gpio_set_dir(DATA_PINS[i], GPIO_IN);

        int bit = gpio_get(DATA_PINS[i]) ? 1 : 0;
        data = (data << 1) + bit;

        // switch data pins back to output mode
        gpio_set_dir(DATA_PINS[i], GPIO_OUT);
    }

    return data;
}

/**
 * Read address sending by 6502 in binary
 * 
 * @return char *
 */
char *rom_read_address_bin() 
{
    static char address[17];
    for (int i = 17; i >= 0; i--) {
        address[i] = gpio_get(ADDRESS_PINS[i]) ? '1' : '0';
    }
    address[16] = '\0';

    return address;
}

/**
 * Read data from rom in binary
 * 
 * @return char *
 */
char *rom_read_data_bin()
{
    static char data[9];
    for (int i = 0; i < 8; i++) {
        // switch data pins to input mode
        gpio_set_dir(DATA_PINS[i], GPIO_IN);

        data[i] = gpio_get(DATA_PINS[i]) ? '1' : '0';

        // switch data pins to input mode
        gpio_set_dir(DATA_PINS[i], GPIO_OUT);
    }
    data[8] = '\0';

    return data;
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
 * @return void
 */
void rom_data_dir_in()
{
    for (int i = 0; i < 8; i++) {
        gpio_set_dir(DATA_PINS[i], GPIO_IN);
    }
}

/**
 * Initialize rom
 * 
 * @return void
 */
void rom_init() 
{
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