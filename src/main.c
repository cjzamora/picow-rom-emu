#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "cmd.h"
#include "rom.h"

int main() 
{
    // initialize stdio
    stdio_init_all();

    // initialize Wi-Fi
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

    // slight delay for initialization
    sleep_ms(1000);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    // initialize rom
    rom_init();
    // initialize cmd
    cmd_init();

    while(true) {
        tight_loop_contents();
    }
}