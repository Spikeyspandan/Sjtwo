#pragma once

#include "FreeRTOS.h"
#include "clock.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "queue.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  UART_2,
  UART_3,
} uart_number_e;

#define UART2_POWER (1U << 24);
#define UART3_POWER (1U << 25);

void uart_lab__init(uart_number_e uart, uint32_t peripheral_clock, uint32_t baud_rate);

// Read the byte from RBR and actually save it to the pointer
bool uart_lab__polled_get(uart_number_e uart, char *input_byte);
bool uart_lab__polled_put(uart_number_e uart, char output_byte);

void uart__enable_receive_interrupt(uart_number_e uart_number);
bool uart_lab__get_char_from_queue(char *input_byte, uint32_t timeout);