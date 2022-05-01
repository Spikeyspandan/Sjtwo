// #include "lcd.h"
// #include "gpio.h"
// #include "uart.h"

// enum Mode { MODE_NORMAL; }

// struct data_buffer {
//   byte data[BUFFER_SIZE];
//   volatile byte head;
//   volatile byte tail;
// } buffer;

// void lcd_init() { uart__init(); }

// void lcd_write(uint8_t data) {
//   gpio__reset(rs);
//   gpio__reset(rw);
//   gpio__reset(cs);

//   lcd_write_task(data);
// }