// #include "FreeRTOS.h"
// #include "adc.h"
// #include "gpio.h"
// #include "pwm1.h"
// #include "queue.h"
// #include "task.h"
// #include <stdio.h>

// // This is the queue handle we will need for the xQueue Send/Receive API
// static QueueHandle_t adc_to_pwm_task_queue;

// uint32_t *gpio__get_iocon(gpio_s gpio);
// void pin_configure_adc_channel_as_io_pin() {

//   gpio_s gpio = gpio__construct_with_function(GPIO__PORT_0, 25, GPIO__FUNCTION_1);
//   volatile uint32_t *pin_iocon = gpio__get_iocon(gpio);

//   const uint32_t mode_mask = UINT32_C(3);
//   *pin_iocon &= ~(mode_mask << 3);
//   *pin_iocon &= ~(1 << 7);
// }

// void pin_configure_pwm_channel_as_io_pin() {
//   LPC_IOCON->P2_0 &= ~0b111;
//   LPC_IOCON->P2_0 |= 0b001;

//   LPC_IOCON->P2_1 &= ~0b111;
//   LPC_IOCON->P2_1 |= 0b001;

//   LPC_IOCON->P2_2 &= ~0b111;
//   LPC_IOCON->P2_2 |= 0b001;

//   gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCTION_1);
//   gpio__construct_with_function(GPIO__PORT_2, 1, GPIO__FUNCTION_1);
//   gpio__construct_with_function(GPIO__PORT_2, 2, GPIO__FUNCTION_1);
// }
// void adc_task(void *p) {
//   // NOTE: Reuse the code from Part 1

//   adc__initialize();

//   // TODO This is the function you need to add to adc.h
//   // You can configure burst mode for just the channel you are using
//   adc__enable_burst_mode();

//   // Configure a pin, such as P1.31 with FUNC 011 to route this pin as ADC channel 5
//   // You can use gpio__construct_with_function() API from gpio.h
//   pin_configure_adc_channel_as_io_pin(); // TODO You need to write this function

//   int adc_reading = 0; // Note that this 'adc_reading' is not the same variable as the one from adc_task
//   while (1) {
//     // Implement code to send potentiometer value on the queue
//     // a) read ADC input to 'int adc_reading'
//     // b) Send to queue: xQueueSend(adc_to_pwm_task_queue, &adc_reading, 0);
//     adc_reading = adc__get_channel_reading_with_burst_mode(ADC__CHANNEL_2);
//     fprintf(stderr, "adc_value = %d\n", adc_reading);
//     float value = 3.3 * ((float)adc_reading / 4095); // adc_voltage / 3.3 = adc_reading / 4095
//     fprintf(stderr, "Value in voltage = %f\n", value);
//     xQueueSend(adc_to_pwm_task_queue, &adc_reading, 0);

//     vTaskDelay(100);
//   }
// }

// void pwm_task(void *p) {
//   // NOTE: Reuse the code from Part 0
//   pwm1__init_single_edge(1000);

//   pin_configure_pwm_channel_as_io_pin();

//   int adc_reading = 0;

//   while (1) {
//     // Read the value in register
//     if (xQueueReceive(adc_to_pwm_task_queue, &adc_reading, 100)) {
//       fprintf(stderr, "MR0 Register: %d\n", LPC_PWM1->MR0); // Match register used to alter PWM LEDs
//       fprintf(stderr, "MR1 Register: %d\n", LPC_PWM1->MR1); // MR1 is used to control P2.0
//       fprintf(stderr, "MR2 Register: %d\n", LPC_PWM1->MR2); // MR1 is used to control P2.1
//       fprintf(stderr, "MR3 Register: %d\n", LPC_PWM1->MR3); // MR1 is used to control P2.2

//       int x = 4095;
//       if (adc_reading < x / 3) {
//         pwm1__set_duty_cycle(PWM1__2_0, adc_reading);
//         pwm1__set_duty_cycle(PWM1__2_1, 0);
//         pwm1__set_duty_cycle(PWM1__2_2, 0);
//       } else if (adc_reading >= x / 3 && adc_reading < (2 * x / 3)) {
//         pwm1__set_duty_cycle(PWM1__2_1, adc_reading);
//         pwm1__set_duty_cycle(PWM1__2_0, 0);
//         pwm1__set_duty_cycle(PWM1__2_2, 0);
//       } else if (adc_reading > (2 * x / 3)) {
//         pwm1__set_duty_cycle(PWM1__2_2, adc_reading);
//         pwm1__set_duty_cycle(PWM1__2_0, 0);
//         pwm1__set_duty_cycle(PWM1__2_1, 0);
//       }
//     }

//     // We do not need task delay because our queue API will put task to sleep when there is no data in the queue
//     // vTaskDelay(100);
//   }
// }

// void main(void) {

//   adc_to_pwm_task_queue = xQueueCreate(1, sizeof(int));

//   xTaskCreate(adc_task, "ADC", 1024, NULL, 0, NULL);
//   xTaskCreate(pwm_task, "PWM", 1024, NULL, 1, NULL);

//   vTaskStartScheduler();
// }