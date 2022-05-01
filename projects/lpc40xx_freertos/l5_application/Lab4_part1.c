// #include "FreeRTOS.h"
// #include "adc.h"
// #include "board_io.h"
// #include "gpio.h"
// #include "task.h"
// #include <stdio.h>
// volatile uint32_t *gpio__get_iocon(gpio_s gpio);
// void pin_configure_adc_channel_as_io_pin() {

//   gpio_s gpio = gpio__construct_with_function(GPIO__PORT_0, 25, GPIO__FUNCTION_1);
//   v uint32_t *pin_iocon = gpio__get_iocon(gpio);

//   const uint32_t mode_mask = UINT32_C(3);
//   *pin_iocon &= ~(mode_mask << 3);
//   *pin_iocon &= ~(1 << 7);
// }
// void adc_task(void *p) {
//   adc__initialize();

//   // TODO This is the function you need to add to adc.h
//   // You can configure burst mode for just the channel you are using
//   adc__enable_burst_mode();

//   // Configure a pin, such as P1.31 with FUNC 011 to route this pin as ADC channel 5
//   // You can use gpio__construct_with_function() API from gpio.h
//   pin_configure_adc_channel_as_io_pin(); // TODO You need to write this function

//   while (1) {
//     // Get the ADC reading using a new routine you created to read an ADC burst reading
//     // TODO: You need to write the implementation of this function
//     const uint16_t adc_value = adc__get_channel_reading_with_burst_mode(ADC__CHANNEL_2);
//     fprintf(stderr, "adc_value = %d\n", adc_value);
//     vTaskDelay(100);
//   }
// }

// void main(void) {
//   xTaskCreate(adc_task, "Part 1 ", 1024, NULL, 1, NULL);
//   vTaskStartScheduler();
// }