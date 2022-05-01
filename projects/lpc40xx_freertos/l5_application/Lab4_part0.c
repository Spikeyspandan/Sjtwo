// #include "FreeRTOS.h"
// #include "gpio.h"
// #include "pwm1.h"
// #include "task.h"

// void pin_configure_pwm_channel_as_io_pin() {
//   LPC_IOCON->P2_0 &= ~0b010;
//   LPC_IOCON->P2_0 |= 0b001;
//   gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCTION_1);
// }
// void pwm_task(void *p) {
//   pwm1__init_single_edge(1000);

//   // Locate a GPIO pin that a PWM channel will control
//   // NOTE You can use gpio__construct_with_function() API from gpio.h
//   // TODO Write this function yourself
//   pin_configure_pwm_channel_as_io_pin();

//   // We only need to set PWM configuration once, and the HW will drive
//   // the GPIO at 1000Hz, and control set its duty cycle to 50%
//   pwm1__set_duty_cycle(PWM1__2_0, 50);

//   // Continue to vary the duty cycle in the loop
//   uint8_t percent = 0;
//   while (1) {
//     pwm1__set_duty_cycle(PWM1__2_0, percent);

//     if (++percent > 100) {
//       percent = 0;
//     }

//     vTaskDelay(200);
//   }
// }

// void main(void) {
//   xTaskCreate(pwm_task, "Part 0", 1024, NULL, 1, NULL);
//   vTaskStartScheduler();
// }