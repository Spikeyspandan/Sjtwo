// #include "FreeRTOS.h"
// #include "gpio.h"
// #include "i2c_slave.h"
// #include "sj2_cli.h"
// #include "task.h"
// #include <stdio.h>
// uint8_t slave_memory[256];

// int main(void) {
//   sj2_cli__init();
//   vTaskStartScheduler();
//   // i2c2__slave_init(0x86);
//   // gpio_s led0 = {2, 3};
//   // gpio_s led1 = {1, 26};

//   // gpio__construct_as_output(led0.port_number, led0.pin_number);
//   // gpio__construct_as_output(led1.port_number, led1.pin_number);

//   // while (1) {
//   //   // LED0
//   //   i2c_slave_callback__read_memory(0, &slave_memory[0]);

//   //   if (slave_memory[0] == 0) {
//   //     gpio__set(led0);
//   //   }

//   //   else {
//   //     gpio__reset(led0);
//   //   }
//   //   // LED1
//   //   i2c_slave_callback__read_memory(1, &slave_memory[1]);
//   //   if (slave_memory[1] == 0)
//   //     gpio__set(led1);
//   //   else
//   //     gpio__reset(led1);
//   // }

//   return 0;
// }
