#include "mp3_decoder.h"
#include "gpio.h"
#include "ssp2.h"
#include "stdio.h"

gpio_s DREQ;
gpio_s X_CS;
gpio_s X_DCS;
gpio_s X_RESET;

void mp3_decoder(void) {
  // Connect MP3 to SPI
  todo_configure_your_ssp2_pin_functions();

  gpio__construct_with_function(GPIO__PORT_0, 16, GPIO__FUNCITON_0_IO_PIN); // X_DCS
  gpio__construct_with_function(GPIO__PORT_0, 17, GPIO__FUNCITON_0_IO_PIN); // Dre1
  gpio__construct_with_function(GPIO__PORT_0, 22, GPIO__FUNCITON_0_IO_PIN); // X_RESET
  gpio__construct_with_function(GPIO__PORT_0, 15, GPIO__FUNCITON_0_IO_PIN); // X_DCS

  DREQ = gpio__construct_as_input(GPIO__PORT_0, 17);
  X_CS = gpio__construct_as_output(GPIO__PORT_0, 15);
  X_DCS = gpio__construct_as_output(GPIO__PORT_0, 16);
  X_RESET = gpio__construct_as_output(GPIO__PORT_0, 22);
}

void mp3_decoder_init(void) {
  gpio__set(X_RESET);
  gpio__set(X_DCS);
  gpio__set(X_CS);

  write_register(0x0B, 0x00, 0x00);
  ssp2__initialize(4800);
}

void read_register(uint8_t address) {
  while (!(gpio__get(DREQ))) {
  }
  uint8_t first_half;
  uint8_t second_half;
  gpio__reset(X_CS);         // Low
  ssp2__exchange_byte(0x03); // Read data
  ssp2__exchange_byte(address);
  ssp2__exchange_byte(first_half);
  ssp2__exchange_byte(second_half);

  gpio__set(X_CS); // High
  while (!(gpio__get(DREQ))) {
  }
}

void write_register(uint8_t address, uint8_t first_half, uint8_t second_half) {
  while (!(gpio__get(DREQ))) {
  }

  gpio__reset(X_CS);         // Low
  ssp2__exchange_byte(0x02); // Write data
  ssp2__exchange_byte(address);
  ssp2__exchange_byte(first_half);
  ssp2__exchange_byte(second_half);

  gpio__set(X_CS); // High
  while (!(gpio__get(DREQ))) {
  }
}

void send_data(char data) {
  gpio__reset(X_DCS);
  ssp2__exchange_byte(data);
  gpio__set(X_DCS);
}

bool Dreq(void) {
  bool Dreq_status = false;
  if (gpio__get(DREQ)) {
    Dreq_status = true;
  }
  return Dreq_status;
}