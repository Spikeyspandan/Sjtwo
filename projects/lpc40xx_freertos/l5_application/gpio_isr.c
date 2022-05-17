// @file gpio_isr.c
#include "gpio_isr.h"
#include "FreeRTOS.h"
#include "lpc40xx.h"
#include <stdint.h>
#include <stdio.h>

static function_pointer_t gpio0_callbacks[32] = {NULL};

static function_pointer_t gpio2_callbacks[32] = {NULL};

void gpio0__attach_interrupt(uint32_t pin, gpio_interrupt_e interrupt_type, function_pointer_t callback) {
  // 1) Store the callback based on the pin at gpio0_callbacks
  gpio0_callbacks[pin] = callback;

  // 2) Configure GPIO 0 pin for rising or falling edge  LPC_GPIO0->DIR &= ~(1 << pin);
  switch (interrupt_type) {
  case GPIO_INTR__FALLING_EDGE:
    LPC_GPIOINT->IO0IntEnF |= (1 << pin);
    break;
  case GPIO_INTR__RISING_EDGE:
    LPC_GPIOINT->IO0IntEnR |= (1 << pin);
    break;
  default:
    break;
  }
}

int get_interrupt_pin_number() {
  unsigned int m = 0, i = 0;

  if (LPC_GPIOINT->IO0IntStatR)
    m = LPC_GPIOINT->IO0IntStatR; // If pin=29. m= 2^29

  else if (LPC_GPIOINT->IO0IntStatF)
    m = LPC_GPIOINT->IO0IntStatF; // If pin=29. m= 2^30

  while (m != 0) {
    m = m >> 1; // Right shift m until m is 0.
    i++;
  }
  fprintf(stderr, "Interrupt on pin %d\n", i - 1);
  return (i - 1);
}

void clear_pin_interrupt(int pin_that_generated_interrupt) {
  LPC_GPIOINT->IO0IntClr |= (1 << pin_that_generated_interrupt);
}

void gpio0__interrupt_dispatcher(void) {
  // Check which pin generated the interrupt
  int pin_that_generated_interrupt = get_interrupt_pin_number();

  function_pointer_t attached_user_handler = gpio0_callbacks[pin_that_generated_interrupt];
  // Invoke the user registered callback, and then clear the interrupt
  attached_user_handler();
  clear_pin_interrupt(pin_that_generated_interrupt);
}

void get_interrupt_pin_number2(int *pin) {
  for (int i = 0; i < 32; i++) {
    if (LPC_GPIOINT->IO2IntStatF & (1 << i)) {
      *pin = i;
    }
  }
}

void gpio2__attach_interrupt(uint32_t pin, gpio_interrupt_e interrupt_type, function_pointer_t callback) {
  gpio2_callbacks[pin] = callback;

  // 2) Configure GPIO 0 pin for rising or falling edge  LPC_GPIO0->DIR &= ~(1 << pin);
  switch (interrupt_type) {
  case GPIO_INTR__FALLING_EDGE:
    LPC_GPIOINT->IO2IntEnF |= (1 << pin);
    break;
  case GPIO_INTR__RISING_EDGE:
    LPC_GPIOINT->IO2IntEnR |= (1 << pin);
    break;
  default:
    break;
  }
}

void clear_pin_interrupt_gpio2(int pin_generated) { LPC_GPIOINT->IO2IntClr = (1 << pin_generated); }

void gpio2__interrupt_dispatcher(void) {

  int pin_that_generated_interrupt;
  get_interrupt_pin_number2(&pin_that_generated_interrupt);

  function_pointer_t attached_user_handler = gpio2_callbacks[pin_that_generated_interrupt];
  // Invoke the user registered callback, and then clear the interrupt
  attached_user_handler();
  clear_pin_interrupt_gpio2(pin_that_generated_interrupt);
}