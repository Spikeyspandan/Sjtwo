// #include "queue.h"
// #include <gpio.h>
// #include <lpc40xx.h>
// #include <stdint.h>
// #include <stdio.h>

// static QueueHandle_t switch_queue;

// typedef enum {
//   switch__off,
//   switch__on
// } switch_e;

// void get_switch_input_from_switch0(void)
// {
//     if(LPC_GPIO0->PIN &(1<<29)) return switch__on;
//     else return switch__off;
// }
// // TODO: Create this task at PRIORITY_LOW
// void producer(void *p) {
//   while (1) {
//     // This xQueueSend() will internally switch context to "consumer" task because it is higher priority than this
//     "producer" task
//     // Then, when the consumer task sleeps, we will resume out of xQueueSend()and go over to the next line

//     // TODO: Get some input value from your board
//     const switch_e switch_value = get_switch_input_from_switch0();

//     // TODO: Print a message before xQueueSend()
//     // Note: Use printf() and not fprintf(stderr, ...) because stderr is a polling printf
//     printf("Producer before xQueueSend %d\n", switch_value);
//     xQueueSend(switch_queue, &switch_value, 0);
//     // TODO: Print a message after xQueueSend()
//     printf("Producer after xQueueSend\n");
//     vTaskDelay(1000);
//   }
// }

// // TODO: Create this task at PRIORITY_HIGH
// void consumer(void *p) {
//   switch_e switch_value;
//   while (1) {

//     // TODO: Print a message before xQueueReceive()
//     printf("Consumer before xQueueReceive\n");
//     xQueueReceive(switch_queue, &switch_value, portMAX_DELAY);
//     // TODO: Print a message after xQueueReceive()
//     printf("Consumer after xQueueReceive")
//   }
// }

// void main(void) {
//   // TODO: Create your tasks
//   xTaskCreate(producer,"producer", 1024, NULL,1, NULL);
//   xTaskCreate(consumer, "consumer", 1024, NULL, 1, NULL);

//   // TODO Queue handle is not valid until you create it
//   switch_queue = xQueueCreate(<depth>, sizeof(switch_e)); // Choose depth of item being our enum (1 should be okay
//   for this example)

//   vTaskStartScheduler();
// }