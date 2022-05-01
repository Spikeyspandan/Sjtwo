// #include "FreeRTOS.h"
// #include "semphr.h"
// #include "ssp2_lab.h"
// #include "stdio.h"
// #include "task.h"
// #include <stdlib.h>

// static SemaphoreHandle_t spi_mutex;

// void spi_id_verification_task(void *p) {
//   while (1) {
//     xSemaphoreGive(spi_mutex);
//     if (xSemaphoreTake(spi_mutex, portMAX_DELAY)) {
//       const adesto_flash_id_s id = adesto_read_signature();
//       fprintf(stderr, "manufacturer_id1 %x\n", id.manufacturer_id);
//       // When we read a manufacturer ID we do not expect, we will kill this task
//       if (0x1F != id.manufacturer_id) {
//         fprintf(stderr, "Manufacturer ID read failure\n");
//         vTaskSuspend(NULL); // Kill this task
//       }
//     }
//     xSemaphoreGive(spi_mutex);
//     vTaskDelay(100);
//   }
// }
// void spi_id_verification_task2(void *p) {
//   while (1) {
//     if (xSemaphoreTake(spi_mutex, portMAX_DELAY)) {
//       const adesto_flash_id_s id = adesto_read_signature();
//       fprintf(stderr, "manufacturer_id2 %x\n", id.manufacturer_id);
//       // When we read a manufacturer ID we do not expect, we will kill this task
//       if (0x1F != id.manufacturer_id) {
//         fprintf(stderr, "Manufacturer ID read failure\n");
//         vTaskSuspend(NULL); // Kill this task
//       }
//     }
//     xSemaphoreGive(spi_mutex);
//     vTaskDelay(100);
//   }
// }
// void main(void) {
//   // TODO: Initialize your SPI, its pins, Adesto flash CS GPIO etc...

//   const uint32_t spi_clock_mhz = 24;
//   ssp2__init(spi_clock_mhz);
//   todo_configure_your_ssp2_pin_functions();

//   spi_mutex = xSemaphoreCreateMutex();
//   // Create two tasks that will continously read signature

//   xTaskCreate(spi_id_verification_task, "SPI", 1024, NULL, 2, NULL);
//   xTaskCreate(spi_id_verification_task2, "SPI", 1024, NULL, 1, NULL);

//   vTaskStartScheduler();
// }
