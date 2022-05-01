// #include "FreeRTOS.h"
// #include "queue.h"
// #include "stdio.h"
// #include "task.h"
// #include "uart_lab.h"

// void uart_read_task(void *p) {
//   while (1) {
//     char input = '0';
//     if (uart_lab__get_char_from_queue(&input, 1000)) {
//       fprintf(stderr, "    RX: %c\n", input);
//     }
//     vTaskDelay(500);
//   }
// }
// void uart_write_task(void *p) {
//   char output = 'A';
//   while (1) {
//     // TODO: Use uart_lab__polled_put() function and send a value

//     if (uart_lab__polled_put(UART_2, output))
//       fprintf(stderr, "TX: %c", output);

//     vTaskDelay(500);
//   }
// }
// void main(void) {
//   uart_lab__init(UART_2, clock__get_peripheral_clock_hz(), 9600);
//   configure_gpio_as_uart(UART_2);
//   uart__enable_receive_interrupt(UART_2);
//   xTaskCreate(uart_read_task, "Read", 4096 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
//   xTaskCreate(uart_write_task, "Write", 4096 / sizeof(void *), NULL, PRIORITY_LOW, NULL);

//   vTaskStartScheduler();
// }