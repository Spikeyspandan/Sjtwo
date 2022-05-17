#include "uart_lab.h"

static QueueHandle_t your_uart_rx_queue;

LPC_UART_TypeDef *temp_uart, *interrupt_uart;

void uart_lab__init(uart_number_e uart, uint32_t peripheral_clock, uint32_t baud_rate) {
  // Refer to LPC User manual and setup the register bits correctly
  // The first page of the UART chapter has good instructions
  // a) Power on Peripheral
  if (uart == UART_2) {
    LPC_SC->PCONP |= UART2_POWER;
  } else if (uart == UART_3)
    LPC_SC->PCONP |= UART3_POWER;

  // b) Setup DLL, DLM, FDR, LCR registers
  temp_uart = uart == UART_2 ? LPC_UART2 : LPC_UART3;
  temp_uart->LCR = 0x83; // Table.401 lpc manual enables DLAB[7] bit and sets read bit size
  temp_uart->FDR = 0x10; // Table.405 lpc manual divAddVal[3:0] = 0 and mulVal[7:4] = 1

  const uint16_t divider_16_bit = peripheral_clock / (16 * baud_rate);
  temp_uart->DLM = (divider_16_bit >> 8) & 0xFF; // Table.395
  temp_uart->DLL = (divider_16_bit >> 0) & 0xFF; // Table.396

  temp_uart->LCR = 0x03; // Disable DLAB[7] bit
}

// Read the byte from RBR and actually save it to the pointer
bool uart_lab__polled_get(uart_number_e uart, char *input_byte) {
  temp_uart = uart == UART_2 ? LPC_UART2 : LPC_UART3;
  // a) Check LSR for Receive Data Ready
  while (!(temp_uart->LSR & 0b1)) // meaning it is not ready yet Table.402
    ;
  // b) Copy data from RBR register to input_byte
  *input_byte = temp_uart->RBR;
  return true;
}

bool uart_lab__polled_put(uart_number_e uart, char output_byte) {
  temp_uart = uart == UART_2 ? LPC_UART2 : LPC_UART3;
  // a) Check LSR for Transmit Hold Register Empty
  while (!(temp_uart->LSR & (0b1 << 5))) // meaning not empty Table.402
    ;
  // b) Copy output_byte to THR register
  temp_uart->THR = output_byte;
  return true;
}

static void your_receive_interrupt(void) {
  // TODO: Read the IIR register to figure out why you got interrupted
  if ((interrupt_uart->IIR & 0b1)) // meaning no interrupt pending Table.398
    return;

  // TODO: Based on IIR status, read the LSR register to confirm if there is data to be read
  if (((interrupt_uart->IIR >> 1) & 0b111) != 0x2) // if it's not a recieve interrupt
    return;
  // TODO: Based on LSR status, read the RBR register and input the data to the RX Queue
  if (!(interrupt_uart->LSR & 0b1)) // if data isn't actually ready
    return;
  const char byte = interrupt_uart->RBR;
  xQueueSendFromISR(your_uart_rx_queue, &byte, NULL);
}

// Public function to enable UART interrupt
// TODO Declare this at the header file
void uart__enable_receive_interrupt(uart_number_e uart_number) {
  // TODO: Use lpc_peripherals.h to attach your interrupt
  IRQn_Type IRQ_en = uart_number == UART_2 ? UART2_IRQn : UART3_IRQn;
  lpc_peripheral_e peripheral_en = uart_number == UART_2 ? LPC_PERIPHERAL__UART2 : LPC_PERIPHERAL__UART3;

  NVIC_EnableIRQ(IRQ_en);
  lpc_peripheral__enable_interrupt(peripheral_en, your_receive_interrupt, "uart");

  // TODO: Enable UART receive interrupt by reading the LPC User manual
  // Hint: Read about the IER register
  temp_uart = uart_number == UART_2 ? LPC_UART2 : LPC_UART3;
  interrupt_uart = temp_uart;
  temp_uart->IER = 0b1; //  enable recieve interrupt
  // TODO: Create your RX queue
  your_uart_rx_queue = xQueueCreate(10, sizeof(char));
}

// Public function to get a char from the queue (this function should work without modification)
// TODO: Declare this at the header file
bool uart_lab__get_char_from_queue(char *input_byte, uint32_t timeout) {
  return xQueueReceive(your_uart_rx_queue, input_byte, timeout);
}