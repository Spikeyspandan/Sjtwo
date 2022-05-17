#include "lcd_driver.h"
#include "clock.h"
#include <stdio.h>
#include <string.h>

const size_t ROW = _ROW;
const size_t COL = _COL;
static const char rows[4] = {0, 64, 20, 84}; // row01 row02 row03 row04 respectively

/* util functions */
static void lcd__set_baud(void) {
  lcd__send_char(0x7c);
  lcd__send_char(0x12); // 230400 baud
}

static void lcd__set_cursor(lcd__row_e row_idx) {
  lcd__send_char(254); // command char Note below: + 0 is for the col position note that it's fixed to the 1st col
  lcd__send_char(128 + rows[row_idx] + 0); // 128 to say cursor control followed by setting the cursor value on the grid
}
/* end of util */

void lcd__init(uint32_t baud_rate) {
  gpio__construct_with_function(GPIO__PORT_0, 0, GPIO__FUNCTION_2); // UART3 TX
  gpio__construct_with_function(GPIO__PORT_0, 1, GPIO__FUNCTION_2); // UART3 RX

  uart_lab__init(UART_3, clock__get_peripheral_clock_hz(), baud_rate);
  lcd__set_baud(); // set to 230400 baud
}

void lcd__send_char(char output_byte) { uart_lab__polled_put(UART_3, output_byte); }

void lcd__send_row(char *walker, lcd__row_e row_idx) {
  size_t size = 0;

  lcd__set_cursor(row_idx); // will always be col 0 only row is a variable position in set cursor func
  while (*walker != '\0' && size < COL) {
    lcd__send_char(*walker);
    walker++;
    size++;
  }
}

void lcd__clr_screen(void) {
  lcd__send_char(0x7c); // enter setting mode
  lcd__send_char(0x2d); // clear command
}
