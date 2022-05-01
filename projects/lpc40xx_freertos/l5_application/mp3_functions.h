#include "FreeRTOS.h"
#include "adc.h"
#include "board_io.h"
#include "gpio.h"
#include "mp3_decoder.h"
#include "task.h"
#include <stdio.h>

void pin_configure_adc_channel_as_io_pin();
int adc_task1();
void volume_control();
void bass_control();
void treble_control();