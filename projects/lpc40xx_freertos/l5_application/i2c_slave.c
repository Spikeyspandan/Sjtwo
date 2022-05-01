
#include "i2c_slave.h"
#include "FreeRTOS.h"
#include "gpio.h"

static volatile uint8_t slave_memory[256];
void i2c2__slave_init(uint8_t slave_address_to_respond_to) {

  LPC_I2C2->ADR0 |= slave_address_to_respond_to;
  LPC_I2C2->MASK0 &= 0;
  LPC_I2C2->CONSET |= (1 << 6) | (1 << 2);
}

bool i2c_slave_callback__read_memory(uint8_t memory_index, uint8_t *memory) {
  if (memory_index < 256) {
    *memory = slave_memory[memory_index];
    return true;
  } else {
    return false;
  }
}

bool i2c_slave_callback__write_memory(uint8_t memory_index, uint8_t memory_value) {
  if (memory_index < 256) {
    slave_memory[memory_index] = memory_value;
    return true;
  } else {
    return false;
  }
}