// #include "gp1o_lab.h"
// #include <stdbool.h>
// #include <stdio.h>

// #include "FreeRTOS.h"
// #include "lpc40xx.h"
// #include "semphr.h"
// #include "task.h"
// static SemaphoreHandle_t switch_press_indication;
// typedef struct {
//   /* First get gpio0 driver to work only, and if you finish it
//    * you can do the extra credit to also make it work for other Ports
//    */
//   uint8_t port;
//   uint8_t pin;
// } port_pin_s;

// void led_task(void *task_parameter) {
//   // Type-cast the paramter that was passed from xTaskCreate()
//   const port_pin_s *led = (port_pin_s *)(task_parameter);

//   while (true) {
//     // Note: There is no vTaskDelay() here, but we use sleep mechanism while waiting for the binary semaphore
//     (signal) if (xSemaphoreTake(switch_press_indication, 1000)) {
//       puts("Switch press");
//       gpio0__set_as_output(led->pin, led->port);
//       gpio0__set_high(led->pin, led->port);
//       vTaskDelay(100);
//       gpio0__set_low(led->pin, led->port);
//       vTaskDelay(100);

//       // TODO: Blink the LED
//     } else {
//       puts("Timeout: No switch press indication for 1000ms");
//     }
//   }
// }

// void switch_task(void *task_parameter) {
//   port_pin_s *switcha = (port_pin_s *)task_parameter;

//   while (true) {
//     gpio0__set_as_input(switcha->pin, switcha->port);
//     // TODO: If switch pressed, set the binary semaphore
//     if (gpio0__get_level(switcha->pin, switcha->port)) {
//       xSemaphoreGive(switch_press_indication);
//     }

//     // Task should always sleep otherwise they will use 100% CPU
//     // This task sleep also helps avoid spurious semaphore give during switch debeounce
//     vTaskDelay(100);
//   }
// }
// void Part3() {
//   switch_press_indication = xSemaphoreCreateBinary();
//   static port_pin_s led0 = {1, 18};
//   static port_pin_s switch1 = {0, 29};

//   xTaskCreate(led_task, "led", 2048 / sizeof(void *), &led0, 1, 0); /* &led0 is a task parameter going to led_task */
//   xTaskCreate(switch_task, "led", 2048 / sizeof(void *), &switch1, 1, 0);
// }
// int main(void) {

//   Part3();
//   vTaskStartScheduler();
//   return 0;
// }