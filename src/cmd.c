#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "cmd.h"
#include "rom.h"

/**
 * Command repeating timer
 * 
 * @var repeating_timer
 */
struct repeating_timer cmd_timer;

/**
 * Command data type
 * 
 * @var cmd_data_t
 */
typedef struct {
    void (*cmd_execute)(char *cmd);
} cmd_data_t;

/**
 * Command data
 * 
 * @var cmd_data_t
 */
cmd_data_t *cmd_data;

/**
 * Boot message
 * 
 * @return void
 */
void cmd_boot_message()
{
    printf("\033[2J\033[1;1H");
    printf("\033[1mPico ROM Emulator\033[0m\n\n");

    cmd_info();

    printf("Type '?' for help\n\n");
}

/**
 * Command info
 * 
 * @return void
 */
void cmd_info()
{
    u_int16_t stack_start = rom_get_stack_start();
    u_int16_t stack_end = rom_get_stack_end();
    u_int32_t rom_len = rom_get_len();

    printf(
        "Stack start: 0x%04X\n"
        "Stack end: 0x%04X\n"
        "ROM size: %lu bytes\n\n",
        stack_start,
        stack_end,
        rom_len
    );

    printf("\n");
}

/**
 * Command help
 * 
 * @return void
 */
void cmd_help()
{
    printf(
        "\n"
        "?\t\tshows this help\n"
        "reboot\t\treboots the pico\n"
        "clear\t\tclears the screen\n"
        "\n"
    );
}

/**
 * Command flush
 * 
 * @return void
 */
void cmd_flush() {
    int ch;
    while ((ch = getchar_timeout_us(0)) != PICO_ERROR_TIMEOUT && ch != '\n' && ch != EOF) {}
}

/**
 * Command execute
 * 
 * @param char *cmd
 * @return void
 */
void cmd_execute(char *cmd)
{
    // stop command timer
    cmd_stop();

    if (strcmp(cmd, "?") == 0) {
        cmd_help();
    } else if (strcmp(cmd, "reboot") == 0) {
        reset_usb_boot(0, 0);
    } else if (strcmp(cmd, "clear") == 0) {
        printf("\033[2J\033[1;1H");
        cmd_boot_message();
    } else {
        printf("Unknown command\n");
    }
    
    // run command timer
    cmd_run();
}

/**
 * Command timer callback
 * 
 * @param repeating_timer_t *t
 * @return bool
 */
bool cmd_timer_callback(repeating_timer_t *t)
{
    // get cmd data
    cmd_data_t *cmd_data = (cmd_data_t *) t->user_data;

    // get character
    int ch = getchar_timeout_us(0);
    // command buffer
    static char cmd_buffer[256];
    // command buffer index
    static unsigned int index = 0;

    // if no character
    if (ch == PICO_ERROR_TIMEOUT) {
        return true;
    }

    // if character is newline or carriage return
    if (ch == '\n' || ch == '\r') {
        // execute command
        cmd_data->cmd_execute(cmd_buffer);
        // flush command buffer
        memset(cmd_buffer, 0, sizeof(char) * 256);
        // reset index
        index = 0;
    } else if (index < sizeof(cmd_buffer) - 1) {
        // add character to command buffer
        cmd_buffer[index++] = ch;
    }

    return true;
}

/**
 * Command stop timer
 * 
 * @return void
 */
void cmd_stop()
{
    cancel_repeating_timer(&cmd_timer);
    free(cmd_data);
}

/**
 * Command run timer
 * 
 * @return void
 */
void cmd_run()
{
    printf(">>> ");

    // create cmd data
    cmd_data = (cmd_data_t *) malloc(sizeof(cmd_data_t));
    // set cmd execute
    cmd_data->cmd_execute = cmd_execute;

    // flush input buffer
    cmd_flush();
    // start repeating timer
    add_repeating_timer_ms(100, cmd_timer_callback, cmd_data, &cmd_timer);
}

/**
 * Command init function
 * 
 * @return void
 */
void cmd_init()
{
    cmd_boot_message();
    cmd_run();
}