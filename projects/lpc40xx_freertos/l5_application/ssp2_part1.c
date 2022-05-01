// #include "FreeRTOS.h"
// #include "gpio.h"
// #include "ssp2_lab.h"
// #include "stdio.h"
// #include "task.h"

// void spi_task(void *p) {
//   const uint32_t spi_clock_mhz = 24;
//   ssp2__init(spi_clock_mhz);

//   // From the LPC schematics pdf, find the pin numbers connected to flash memory
//   // Read table 84 from LPC User Manual and configure PIN functions for SPI2 pins
//   // You can use gpio__construct_with_function() API from gpio.h
//   //
//   // Note: Configure only SCK2, MOSI2, MISO2.
//   // CS will be a GPIO output pin(configure and setup direction)
//   todo_configure_your_ssp2_pin_functions();

//   while (1) {
//     adesto_flash_id_s id = adesto_read_signature();
//     fprintf(stderr, "Manufacture id %x\n", id.manufacturer_id);
//     fprintf(stderr, "Manufacture id %x\n", id.device_id_1);
//     fprintf(stderr, "Manufacture id %x\n", id.device_id_2);
//     // TODO: printf the members of the 'adesto_flash_id_s' struct

//     vTaskDelay(500);
//   }
// }

// void main(void) {
//   xTaskCreate(spi_task, "SPI", 1024, NULL, 1, NULL);
//   vTaskStartScheduler();
// }