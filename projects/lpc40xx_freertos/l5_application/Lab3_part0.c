// #include "FreeRTOS.h"
// #include "board_io.h"
// #include "common_macros.h"
// #include "gpio_isr.h"
// #include "periodic_scheduler.h"
// #include "semphr.h"
// #include "task.h"
// #include <stdio.h>
// void main(void) {
//   // For example, choose SW2 (P0_30) pin on SJ2 board and configure as input

//   LPC_GPIO0->DIR &= ~(1 << 30);
//   LPC_IOCON->P0_30 &= ~(UINT32_C(3) << 3);
//   LPC_IOCON->P0_30 |= (UINT32_C(1) << 3);

//   // Configure the registers to trigger Port0 interrupt (such as falling edge)

//   LPC_GPIOINT->IO0IntEnF |= (1 << 30);

//   // Install GPIO interrupt function at the CPU interrupt (exception) vector
//   // c) Hijack the interrupt vector at interrupt_vector_table.c and have it call our gpio_interrupt()
//   //    Hint: You can declare 'void gpio_interrupt(void)' at interrupt_vector_table.c such that it can see this

//   // Most important step: Enable the GPIO interrupt exception using the ARM Cortex M API (this is from lpc40xx.h)
//   NVIC_EnableIRQ(GPIO_IRQn);
//   gpio_interrupt();
//   // Toggle an LED in a loop to ensure/test that the interrupt is entering ane exiting
//   // For example, if the GPIO interrupt gets stuck, this LED will stop blinking

//   while (1) {
//     // TODO: Toggle an LED here

//     LPC_GPIO1->PIN |= 1 << 24;
//     delay__ms(100);
//     LPC_GPIO1->PIN &= ~(1 << 24);
//     delay__ms(100);
//   }
// }
// void gpio_interrupt(void) {
//   LPC_GPIOINT->IO0IntClr |= (1 << 30);
//   fprintf(stderr, "ISR\n");
// }