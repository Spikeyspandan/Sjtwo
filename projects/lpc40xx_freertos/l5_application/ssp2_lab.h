#include "lpc40xx.h"
#include <stdint.h>

typedef struct {
  uint8_t manufacturer_id;
  uint8_t device_id_1;
  uint8_t device_id_2;
  uint8_t extended_device_id;
} adesto_flash_id_s;
adesto_flash_id_s adesto_read_signature(void);
void ssp2__init(uint32_t max_clock_mhz);
uint8_t ssp2__exchange_byte2(uint8_t data_out);
void todo_configure_your_ssp2_pin_functions();
void adesto_cs(void);
void adesto_ds(void);