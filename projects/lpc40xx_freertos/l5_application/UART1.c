// #include "FreeRTOS.h"
// #include "stdio.h"
// #include "task.h"
// #include "uart_lab.h"

// void uart_read_task(void *p) {
//   while (1) {
//     // TODO: Use uart_lab__polled_get() function and printf the received value
//     char input = 0;
//     uart_lab__polled_get(UART_2, &input);
//     fprintf(stderr, "Input value %c\n", input);
//     vTaskDelay(500);
//   }
// }

// void uart_write_task(void *p) {
//   while (1) {
//     // TODO: Use uart_lab__polled_put() function and send a value
//     char output = 'A';
//     fprintf(stderr, "Data out %c\n", output);
//     uart_lab__polled_put(UART_2, output);
//     vTaskDelay(500);
//   }
// }

// void main(void) {
//   // TODO: Use uart_lab__init() function and initialize UART2 or UART3 (your choice)
//   // TODO: Pin Configure IO pins to perform UART2/UART3 function
//   uart_lab__init(UART_2, clock__get_peripheral_clock_hz(), 9600);
//   configure_gpio_as_uart(UART_2);
//   xTaskCreate(uart_read_task, "Read", 1024, NULL, 1, NULL);
//   xTaskCreate(uart_write_task, "Write", 1024, NULL, 2, NULL);

//   vTaskStartScheduler();
// }