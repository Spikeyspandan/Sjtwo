#include <stdbool.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "ff.h"
#include "gpio.h"
#include "queue.h"
#include "semphr.h"

gpio_s SW1, SW2, SW3, SW4, SW5, ENCA, ENCB;

int no_of_songs();
void play_song();
void pause_song();
void play_song_from_home();

void pause_song_ISR();
void next_song();
void next_song_ISR();

void previous_song();
void previous_song_ISR();
void buttons();

void Control_Menu_Print();
void Enter_control_mode();

void move_up_list();
void move_down_list();

void regular_menu_interrupts();
uint32_t get_rotary_position(void);
void encoder__turn_on_power(void);
void rotary_index_max_position(void);

void mp3_pins_init(void);
void encoder__init(void);