#ifndef ROM_H
#define ROM_H

/**
 * Read address from address pins
 * 
 * @return u_int16_t
 */
u_int16_t rom_read_address();

/**
 * Read data from data pins
 * 
 * @return u_int8_t
 */
u_int8_t rom_read_data();

/**
 * Read address from address pins in binary
 * 
 * @return char *
 */
char *rom_read_address_bin();

/**
 * Read data from rom in binary
 * 
 * @return char *
 */
char *rom_read_data_bin();

/**
 * Read data from data pins
 * 
 * @return u_int8_t
 */
void rom_write_data(u_int8_t data);

/**
 * Check if rom is reading
 * 
 * @return bool
 */
bool rom_is_read_req();

/**
 * Set data pins to output mode
 * 
 * @return void
 */
void rom_data_dir_out();

/**
 * Set data pins to input mode
 * 
 * @return void
 */
void rom_data_dir_in();

/**
 * Rom init function
 * 
 * @return void
 */
void rom_init();

#endif