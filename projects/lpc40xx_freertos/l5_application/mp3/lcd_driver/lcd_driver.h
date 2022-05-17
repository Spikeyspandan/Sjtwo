#pragma once

#include <stdint.h>

#include "gpio.h"
#include "uart_lab.h"

#define _ROW 4
#define _COL 20

typedef enum {
  ROW_01 = 0,
  ROW_02,
  ROW_03,
  ROW_04,
} lcd__row_e;

void lcd__init(uint32_t baud_rate);
void lcd__send_char(char output_byte);
void lcd__send_row(char *walker, lcd__row_e row_idx);
void lcd__clr_screen(void);
