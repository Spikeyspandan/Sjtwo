#include "mp3_buttons.h"
#include "gpio.h"
#include "gpio_isr.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "mp3.h"
#include "queue.h"
#include "semphr.h"
#include "song_list.h"
#include "volume_treble.h"
#include <stdio.h>

extern QueueHandle_t mp3_data_transfer_queue;
extern QueueHandle_t songname_queue;

SemaphoreHandle_t mp3_previous;
SemaphoreHandle_t mp3_next;
SemaphoreHandle_t mp3_pause_play;
SemaphoreHandle_t mp3_control_menu;
SemaphoreHandle_t mp3_bass_control;
SemaphoreHandle_t mp3_volume_control;
SemaphoreHandle_t mp3_treble_control;
SemaphoreHandle_t song_list_up;
SemaphoreHandle_t song_list_down;

bool interrupt = false;

volatile size_t number_of_songs;
volatile size_t song_index = 0;
extern TaskHandle_t mp3_player_handle;

volatile size_t current_song = false;
volatile bool first_song = true;
volatile bool change_song = false;
volatile bool previous_song_index = 0;
volatile bool pause_button = false;
volatile bool menu_button = false;
volatile bool currently_playing = false;
volatile int menu_check = 0;

gpio_s SW1, SW2, SW3, SW4, SW5, ENCA, ENCB, Control_Menu;

int no_of_song() {
  number_of_songs = song_list__get_item_count();
  return number_of_songs;
}

void pause_song_ISR(void) {
  if (interrupt == false) {
    interrupt = true;
    pause_button = !pause_button;
    xSemaphoreGiveFromISR(mp3_pause_play, NULL);
  }
}

void pause_song(void) {
  currently_playing = false;
  vTaskSuspend(mp3_player_handle);
}

void play_song(void) {
  currently_playing = true;
  vTaskResume(mp3_player_handle);
}

void play_song_from_home(void) {

  if (first_song) {
    first_song = false;
  } else {
    change_song = true;
  }
  currently_playing = true;
  song_playing(song_index);
  xQueueSend(songname_queue, song_list__get_name_for_item(song_index), 0);
}

void next_song_ISR(void) {
  if (interrupt == false) {
    interrupt = true;
    currently_playing = true;
    xSemaphoreGiveFromISR(mp3_next, NULL);
  }
}

void next_song(void) {

  if (first_song) {
    first_song = false;
  } else {
    change_song = true;
  }
  if (song_index >= (no_of_song() - 1)) {
    song_index = 0;
  } else {

    song_index = (song_index + 1);
  }
  song_playing(song_index);
  xQueueSend(songname_queue, song_list__get_name_for_item(song_index), 0);
}

void previous_song_ISR(void) {
  if (interrupt == false) {
    interrupt = true;
    currently_playing = true;
    xSemaphoreGiveFromISR(mp3_previous, NULL);
  }
}

void previous_song(void) {

  if (first_song) {
    first_song = false;
  } else {
    previous_song_index = true;
  }
  if (song_index == 0) {
    song_index = (no_of_song() - 1);
  } else {
    song_index = (song_index - 1);
  }
  song_playing(song_index);
  xQueueSend(songname_queue, song_list__get_name_for_item(song_index), 0);
}

void Control_Menu_Print(void) {
  if (interrupt == false) {
    menu_button = !menu_button;
    interrupt = true;
    xSemaphoreGiveFromISR(mp3_control_menu, NULL);
  }
}

void left_as_volume() {
  if (interrupt == false) {
    interrupt = true;
    xSemaphoreGiveFromISR(mp3_volume_control, NULL);
  }
}

void right_as_bass() {
  if (interrupt == false) {
    interrupt = true;
    xSemaphoreGiveFromISR(mp3_bass_control, NULL);
  }
}

void down_as_treble() {
  if (interrupt == false) {
    interrupt = true;
    xSemaphoreGiveFromISR(mp3_treble_control, NULL);
  }
}

void move_up_list_ISR(void) {
  if (interrupt == false) {
    interrupt = true;
    xSemaphoreGiveFromISR(song_list_up, NULL);
  }
}

void move_down_list_ISR(void) {
  if (interrupt == false) {
    interrupt = true;
    xSemaphoreGiveFromISR(song_list_down, NULL);
  }
}

void move_up_list(void) {
  currently_playing = false;
  if (song_index == 0) {

    mp3__init_lcd_display(0);
  } else if (song_index > 0) {
    song_index = song_index - 1;
    int song_size = song_index;
    mp3__init_lcd_display(song_size);
  }
}

void move_down_list(void) {
  currently_playing = false;
  if (song_index <= (no_of_song())) {
    song_index = song_index + 1;
    int song_size = song_index;

    if (song_size > (no_of_song() - 1)) {
      song_size = (no_of_song() - 1);
    }
    mp3__init_lcd_display(song_size);
  }
}

void Enter_control_mode(void) {
  mp3_controls_display();
  gpio2__attach_interrupt(2, GPIO_INTR__FALLING_EDGE, left_as_volume);
  gpio2__attach_interrupt(5, GPIO_INTR__FALLING_EDGE, right_as_bass);
  gpio2__attach_interrupt(2, GPIO_INTR__FALLING_EDGE, down_as_treble);
}
void encoder__init() {
  buttons();
  encoder__turn_on_power();

  LPC_QEI->MAXPOS = 1;
}
void mp3_pins_init() {

  mp3_previous = xSemaphoreCreateBinary();
  mp3_next = xSemaphoreCreateBinary();
  mp3_pause_play = xSemaphoreCreateBinary();
  mp3_control_menu = xSemaphoreCreateBinary();
  mp3_volume_control = xSemaphoreCreateBinary();
  mp3_bass_control = xSemaphoreCreateBinary();
  mp3_treble_control = xSemaphoreCreateBinary();
  song_list_up = xSemaphoreCreateBinary();
  song_list_down = xSemaphoreCreateBinary();

  buttons();
  regular_menu_interrupts();
  encoder__turn_on_power();
}
void regular_menu_interrupts() {
  gpio2__attach_interrupt(2, GPIO_INTR__FALLING_EDGE, pause_song_ISR);
  gpio2__attach_interrupt(1, GPIO_INTR__FALLING_EDGE, move_up_list_ISR);
  gpio2__attach_interrupt(5, GPIO_INTR__FALLING_EDGE, next_song_ISR);
  gpio2__attach_interrupt(4, GPIO_INTR__FALLING_EDGE, previous_song_ISR);
  gpio2__attach_interrupt(6, GPIO_INTR__FALLING_EDGE, move_down_list_ISR);

  gpio2__attach_interrupt(8, GPIO_INTR__FALLING_EDGE, Control_Menu_Print);

  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, gpio2__interrupt_dispatcher, "name");
}

void buttons(void) {

  // SW1= center
  SW1 = gpio__construct_with_function(GPIO__PORT_2, 2, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_as_input(SW1);
  // SW2= Down
  SW2 = gpio__construct_with_function(GPIO__PORT_2, 1, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_as_input(SW2);
  // SW3= Right
  SW3 = gpio__construct_with_function(GPIO__PORT_2, 5, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_as_input(SW3);
  // SW4= Up
  SW4 = gpio__construct_with_function(GPIO__PORT_2, 6, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_as_input(SW4);
  // SW5= Left
  SW5 = gpio__construct_with_function(GPIO__PORT_2, 4, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_as_input(SW5);

  Control_Menu = gpio__construct_with_function(GPIO__PORT_2, 8, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_as_input(Control_Menu);

  ENCA = gpio__construct_with_function(GPIO__PORT_1, 20, GPIO__FUNCTION_3);
  ENCB = gpio__construct_with_function(GPIO__PORT_1, 23, GPIO__FUNCTION_3);
}

uint32_t get_rotary_position(void) { return (LPC_QEI->INXCNT); }

void encoder__turn_on_power(void) { lpc_peripheral__turn_on_power_to(LPC_PERIPHERAL__QEI); }
void encoder__set_max_position(void) {}