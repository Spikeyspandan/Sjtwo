// #include <stdlib.h>

// // This task is done for you, but you should understand what this code is doing
// void board_1_sender_task(void *p) {
//   char number_as_string[16] = { 0 };

//    while (true) {
//      const int number = rand();
//      sprintf(number_as_string, "%i", number);

//      // Send one char at a time to the other board including terminating NULL char
//      for (int i = 0; i <= strlen(number_as_string); i++) {
//        uart_lab__polled_put(UART_2,number_as_string[i]);
//        printf("Sent: %c\n", number_as_string[i]);
//      }

//      printf("Sent: %i over UART to the other board\n", number);
//      vTaskDelay(3000);
//    }
// }

// void board_2_receiver_task(void *p) {
//   char number_as_string[16] = { 0 };
//   int counter = 0;

//   while (true) {
//     char byte = 0;
//     uart_lab__get_char_from_queue(&byte, portMAX_DELAY);
//     printf("Received: %c\n", byte);

//     // This is the last char, so print the number
//     if ('\0' == byte) {
//       number_as_string[counter] = '\0';
//       counter = 0;
//       printf("Received this number from the other board: %s\n", number_as_string);
//     }
//     // We have not yet received the NULL '\0' char, so buffer the data
//     else {
//       // TODO: Store data to number_as_string[] array one char at a time
//       // Hint: Use counter as an index, and increment it as long as we do not reach max value of 16
//       number_as_string[counter] = byte;
//       counter++;

//       if(counter == 16) counter =0;
//     }
//   }
// }

// void main (void)
// {
//   uart_lab__init(UART_2, clock__get_peripheral_clock_hz(), 9600);
//   configure_gpio_as_uart(UART_2);
//   uart__enable_receive_interrupt(UART_2);
//   xTaskCreate(board_1_sender_task, "TX", 4096/sizeof(void *), NULL, PRIORITY_LOW, NULL);
//   xTaskCreate(board_2_receiver_task, "RX", 4096/sizeof(void *), NULL, PRIORITY_LOW, NULL);

//   vTaskStartScheduler();
// }