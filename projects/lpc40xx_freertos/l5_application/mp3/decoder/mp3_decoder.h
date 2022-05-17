#pragma once
#include "gpio.h"

void mp3_decoder(void);

void mp3_decoder_init(void);

void read_register(uint8_t register_ad);

void write_register(uint8_t register_ad, uint8_t first_half, uint8_t second_half);

bool Dreq(void);

void send_data(char data);