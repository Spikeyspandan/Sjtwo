#include "gp1o_lab.h"
#include "lpc40xx.h"

void gpio0__set_as_input(uint8_t pin, uint8_t port) {

  switch (port) {
  case 0:
    LPC_GPIO0->DIR &= ~(1 << pin);
    break;
  case 1:
    LPC_GPIO1->DIR &= ~(1 << pin);
    break;
  case 2:
    LPC_GPIO2->DIR &= ~(1 << pin);
    break;

  default:
    break;
  }
}

/// Should alter the hardware registers to set the pin as output
void gpio0__set_as_output(uint8_t pin, uint8_t port) {
  switch (port) {
  case 0:
    LPC_GPIO0->DIR |= (1 << pin);
    break;
  case 1:
    LPC_GPIO1->DIR |= (1 << pin);
    break;
  case 2:
    LPC_GPIO2->DIR |= (1 << pin);
    break;

  default:
    break;
  }
}

/// Should alter the hardware registers to set the pin as high
void gpio0__set_high(uint8_t pin, uint8_t port) {
  switch (port) {
  case 0:
    LPC_GPIO0->PIN |= (1 << pin);
    break;
  case 1:
    LPC_GPIO1->PIN |= (1 << pin);
    break;
  case 2:
    LPC_GPIO2->PIN |= (1 << pin);
    break;

  default:
    break;
  }
}

/// Should alter the hardware registers to set the pin as low
void gpio0__set_low(uint8_t pin, uint8_t port) {

  switch (port) {
  case 0:
    LPC_GPIO0->PIN &= ~(1 << pin);
    break;
  case 1:
    LPC_GPIO1->PIN &= ~(1 << pin);
    break;
  case 2:
    LPC_GPIO2->PIN &= ~(1 << pin);
    break;

  default:
    break;
  }
}

void gpio0__set(uint8_t pin, uint8_t port, bool high) {
  switch ((int)high) {
  case 1:
    gpio0__set_high(pin, port);
    break;
  case 2:
    gpio0__set_low(pin, port);
    break;

  default:
    break;
  }
}

bool gpio0__get_level(uint8_t pin, uint8_t port) {
  bool level = false;
  switch (port) {
  case 0:
    if (LPC_GPIO0->PIN & (1 << pin)) {
      level = true;
    }
    break;
  case 1:
    if (LPC_GPIO1->PIN & (1 << pin)) {
      level = true;
    }
    break;
  case 2:
    if (LPC_GPIO2->PIN & (1 << pin)) {
      level = true;
    }
    break;

  default:
    break;
  }

  return level;
}
