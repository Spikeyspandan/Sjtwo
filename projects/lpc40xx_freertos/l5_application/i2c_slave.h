#include <lpc40xx.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "gpio.h"

void i2c2__slave_init(uint8_t slave_address_to_respond_to);

bool i2c_slave_callback__read_memory(uint8_t memory_index, uint8_t *memory);

bool i2c_slave_callback__write_memory(uint8_t memory_index, uint8_t memory_value);