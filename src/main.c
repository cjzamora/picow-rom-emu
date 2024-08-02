#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/timer.h"
#include "rom.h"
#include "rom_bin.h"

// define clock pin
const int CLOCK_PIN = 28;

/**
 * Handle interrupt
 * 
 * @param uint gpio
 * @param uint32_t events
 * @return void
 */
void handle_interrupt(uint gpio, uint32_t events) 
{
    if (gpio == CLOCK_PIN && events == GPIO_IRQ_EDGE_RISE) {
        // read address and data from 6502
        u_int16_t address = rom_read_address();
        // if 6502 is reading, read data from rom
        u_int8_t data = rom_read_data();

        // read address and data in binary
        char *address_bin = rom_read_address_bin();
        // read data in binary
        char *data_bin = rom_read_data_bin();

        // determine if 6502 is reading or writting
        char mode = rom_is_read_req() ? 'r' : 'W';

        // if 6502 is writting, set the data pins to high impedance
        // or low to prevent the 6502 from reading the data bus
        if (mode == 'W') {
            rom_data_dir_in(true);
        } else {
            rom_data_dir_out();
        }

        // DEBUG: print address, data, and mode
        printf("%s    %s    %04x   %c   %02x\n", address_bin, data_bin, address, mode, data);
    }
}

int main() 
{
    // initialize stdio
    stdio_init_all();

    // initialize Wi-Fi
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

    // initialize clock pin
    gpio_init(CLOCK_PIN);
    // set clock pin to input mode
    gpio_set_dir(CLOCK_PIN, GPIO_IN);
    // add clock interrupt
    gpio_set_irq_enabled_with_callback(CLOCK_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &handle_interrupt);

    // initialize rom
    rom_init();

    while(true) {
        // read address from 6502
        u_int16_t address = rom_read_address();

        // if 6502 is reading, read data from rom
        if (rom_is_read_req()) {
            // read data from rom
            u_int8_t data = rom_bin[address];
            // write data to 6502
            rom_write_data(data);
            // wait for 1 microsecond
            sleep_us(1);
        }
    }
}