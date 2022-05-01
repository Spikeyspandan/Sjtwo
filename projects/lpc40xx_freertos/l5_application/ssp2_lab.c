#include "ssp2_lab.h"
#include <clock.h>
#include <gpio.h>
#include <lpc40xx.h>
#include <stdint.h>
#include <stdio.h>
void ssp2__init(uint32_t max_clock_mhz) {
  // Refer to LPC User manual and setup the register bits correctly
  // a) Power on Peripheral
  LPC_SC->PCONP |= (1 << 20); // bit 20 is for Power on
  // b) Setup control registers CR0 and CR1
  LPC_SSP2->CR0 = (0b111 << 0) | (0 << 8); // Serial Clock Rate and 8 bit transfer
  LPC_SSP2->CR1 = (1 << 1);                // SSP enable

  // c) Setup prescalar register to be <= max_clock_mhz
  uint32_t cpu_clock = clock__get_core_clock_hz() / 1000UL;
  uint8_t divider = 2;

  while (max_clock_mhz < (cpu_clock / divider)) {
    divider += 2;
  }
  LPC_SSP2->CPSR = divider; // 96/24
}

// TODO: Implement the code to read Adesto flash memory signature
// TODO: Create struct of type 'adesto_flash_id_s' and return it
adesto_flash_id_s adesto_read_signature(void) {
  adesto_flash_id_s data = {0};

  adesto_cs();
  {
    // Send opcode and read bytes
    uint8_t sent_data = 0x9F;
    // TODO: Populate members of the 'adesto_flash_id_s' struct
    ssp2__exchange_byte2(sent_data);
    data.manufacturer_id = ssp2__exchange_byte2(0x0F);
    data.device_id_1 = ssp2__exchange_byte2(0x0F);
    data.device_id_2 = ssp2__exchange_byte2(0x0F);
  }
  adesto_ds();

  return data;
}

uint8_t ssp2__exchange_byte2(uint8_t data_out) {
  // Configure the Data register(DR) to send and receive data by checking the SPI peripheral status register
  LPC_SSP2->DR = data_out; // reads data

  // Wait and check if its busy
  while (LPC_SSP2->SR & (1 << 4)) {
  }
  return (uint8_t)(LPC_SSP2->DR & 0xFF);
}

void todo_configure_your_ssp2_pin_functions() {
  gpio__construct_with_function(GPIO__PORT_1, 0, GPIO__FUNCTION_4); // CLK P1_0
  gpio__construct_with_function(GPIO__PORT_1, 1, GPIO__FUNCTION_4); // MOSI P1_1
  gpio__construct_with_function(GPIO__PORT_1, 4, GPIO__FUNCTION_4); // MISO P1_4
}
void adesto_cs(void) {
  // Flash CS
  LPC_GPIO1->DIR |= (1 << 10); // P1_10 as output
  LPC_GPIO1->PIN &= ~(1 << 10);

  // Trigger connected to SJboard and logic analyzer
  LPC_GPIO0->DIR |= (1 << 7); // P0_7 as output
  LPC_GPIO0->PIN &= ~(1 << 7);
}
void adesto_ds(void) {

  LPC_GPIO1->PIN |= (1 << 10);
  LPC_GPIO0->PIN |= (1 << 7);
}