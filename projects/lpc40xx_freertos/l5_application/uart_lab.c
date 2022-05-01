#pragma once

#include "uart_lab.h"
#include "FreeRTOS.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "queue.h"
#include <stdbool.h>
#include <stdint.h>
void uart_lab__init(uart_number_e uart, uint32_t peripheral_clock, uint32_t baud_rate) {
  // Refer to LPC User manual and setup the register bits correctly
  // The first page of the UART chapter has good instructions
  // a) Power on Peripheral
  // b) Setup DLL, DLM, FDR, LCR registers

  uint16_t divider_rate = peripheral_clock / (16 * baud_rate);
  if (uart == UART_2) {
    LPC_SC->PCONP |= 1 << 24;
    NVIC_EnableIRQ(UART_2);
    LPC_UART2->DLM = (divider_rate >> 8) & 0xFF;
    LPC_UART2->DLL = (divider_rate)&0xFF;

    LPC_UART2->LCR = (1 << 7);
    LPC_UART2->LCR |= 3 << 0;
    LPC_UART2->LCR &= ~(1 << 7);

  } else if (uart == UART_3) {
    LPC_SC->PCONP |= 1 << 25;

    LPC_UART3->DLM = (divider_rate >> 8) & 0xFF;
    LPC_UART3->DLL = (divider_rate)&0xFF;

    LPC_UART2->LCR = (1 << 7);
    LPC_UART3->LCR |= 3 << 0;
    LPC_UART2->LCR &= ~(1 << 7);
  }
}

// Read the byte from RBR and actually save it to the pointer
bool uart_lab__polled_get(uart_number_e uart, char *input_byte) {
  // a) Check LSR for Receive Data Ready
  // b) Copy data from RBR register to input_byte

  if (uart == UART_2) {
    while (!(LPC_UART2->LSR & 1))
      ;
    *input_byte = (LPC_UART2->RBR & 0xFF);
    return true;
  }

  else if (uart == UART_3) {

    while (!(LPC_UART3->LSR & 1))
      ;
    *input_byte = (LPC_UART3->RBR & 0xFF);
    return true;
  }
}

bool uart_lab__polled_put(uart_number_e uart, char output_byte) {
  // a) Check LSR for Transmit Hold Register Empty
  // b) Copy output_byte to THR register
  const uint32_t thre_bit = (1 << 5);
  if (uart == UART_2) {
    LPC_UART2->LCR &= ~(1 << 7); // dlab disable
    while (!(LPC_UART2->LSR & thre_bit))
      ;
    LPC_UART2->THR = output_byte;
    return true;
  }

  else if (uart == UART_3) {
    LPC_UART3->LCR &= ~(1 << 7); // dlab disable
    while (!(LPC_UART3->LSR & thre_bit))
      ;
    LPC_UART3->THR = output_byte;
    return true;
  }
}

void configure_gpio_as_uart(uart_number_e uart) {
  if (uart == UART_2) {
    gpio__construct_with_function(GPIO__PORT_2, 8, GPIO__FUNCTION_2); // P2.8=> U2_TXD
    gpio__construct_with_function(GPIO__PORT_2, 9, GPIO__FUNCTION_2); // P2.9 => U2_RXD
  }

  else if (uart == UART_3) {
    gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // P4.28=> U3_TXD
    gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // P4.29=> U3_RXD
  }
}

// Private queue handle of our uart_lab.c
static QueueHandle_t your_uart_rx_queue;

// Private function of our uart_lab.c
static void your_receive_interrupt(void) {

  uint32_t interrupt = 2;

  // TODO: Read the IIR register to figure out why you got interrupted
  if (!(LPC_UART2->IIR & (1 << 0)))
  // if 1-> no interrupt pending
  // if 0->at least one interrupt
  {
    if (interrupt == ((LPC_UART2->IIR >> 1) & 0X07)) // FIrst three bit is interrupt identification
    // if that is equal to 2, that means recieve data available
    {
      if ((LPC_UART2->LSR & (1 << 0)))
      // Receiver Data Ready is 1 => RX Data is ready and waiting.
      {
        const char byte = LPC_UART2->RBR;
        xQueueSendFromISR(your_uart_rx_queue, &byte, NULL);
      }
    }
    // TODO: Based on IIR status, read the LSR register to confirm if there is data to be read

    // TODO: Based on LSR status, read the RBR register and input the data to the RX Queue
  }
}

// Public function to enable UART interrupt
// TODO Declare this at the header file
void uart__enable_receive_interrupt(uart_number_e uart_number) {
  // TODO: Use lpc_peripherals.h to attach your interrupt
  if (uart_number == UART_2) {
    lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__UART2, your_receive_interrupt, NULL);
    // TODO: Enable UART receive interrupt by reading the LPC User manual
    // Hint: Read about the IER register
    LPC_UART2->IER |= 1 << 0;
  }

  else if (uart_number == UART_3) {
    lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__UART3, your_receive_interrupt, NULL);
    // TODO: Enable UART receive interrupt by reading the LPC User manual
    // Hint: Read about the IER register
    LPC_UART3->IER |= 1 << 0;
  }
  // TODO: Create your RX queue
  your_uart_rx_queue = xQueueCreate(1, sizeof(char));
}

// Public function to get a char from the queue (this function should work without modification)
// TODO: Declare this at the header file
bool uart_lab__get_char_from_queue(char *input_byte, uint32_t timeout) {
  return xQueueReceive(your_uart_rx_queue, input_byte, timeout);
}