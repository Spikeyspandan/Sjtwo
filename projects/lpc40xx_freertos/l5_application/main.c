#include "FreeRTOS.h"
#include "ff.h"
#include "gpio_isr.h"
#include "lcd_driver.h"
#include "lpc_peripherals.h"
#include "mp3.h"
#include "mp3_buttons.h"
#include "mp3_decoder.h"
#include "queue.h"
#include "sj2_cli.h"
#include "song_list.h"
#include "ssp2.h"
#include "stdio.h"
#include "string.h"
#include "task.h"
#include "volume_treble.h"

typedef char songname_t[128];
typedef char song_data_t[512];

QueueHandle_t mp3_data_transfer_queue;
QueueHandle_t songname_queue;

TaskHandle_t mp3_player_handle;

extern SemaphoreHandle_t mp3_previous;
extern SemaphoreHandle_t mp3_next;
extern SemaphoreHandle_t mp3_pause_play;
extern SemaphoreHandle_t mp3_control_menu;
extern SemaphoreHandle_t mp3_bass_control;
extern SemaphoreHandle_t mp3_volume_control;
extern SemaphoreHandle_t mp3_treble_control;
extern SemaphoreHandle_t song_list_up;
extern SemaphoreHandle_t song_list_down;

extern bool interrupt;
volatile bool finished_playing = false;

volatile size_t number_of_songs;
volatile size_t song_list_index = 0;

extern volatile size_t current_song;
extern volatile bool first_song;
extern volatile bool change_song;
extern volatile bool previous_song_index;
extern volatile bool pause_button;
extern volatile bool menu_button;
extern volatile bool currently_playing;

extern volatile int menu_check;

static void open_mp3_file(const char *filename) {
  FIL file;
  UINT br;
  printf("Received song to play; %s\n", filename);
  FRESULT result = f_open(&file, filename, FA_READ);
  if (FR_OK == result) {
    song_data_t buffer = {};

    while (1) {

      if (FR_OK == f_read(&file, buffer, sizeof(buffer), &br)) {

        if (br == 0 || (uxQueueMessagesWaiting(songname_queue) == 1)) {
          break;
        }
        xQueueSend(mp3_data_transfer_queue, buffer, portMAX_DELAY);
      }
    }
  } else {
    printf("Error while opening the song %s", filename);
  }
  f_close(&file);
}

static void mp3_reader_task(void *parameter) {
  songname_t songname = {};
  while (1) {

    if (xQueueReceive(songname_queue, &songname, portMAX_DELAY)) {
      open_mp3_file(songname);
    }
  }
}

static void transfer_mp3(song_data_t songdata) {
  for (size_t index = 0; index < sizeof(song_data_t); index++) {
    while (!(Dreq())) {
      vTaskDelay(1);
    }
    send_data(songdata[index]);
  }
}
static void mp3_player_task(void *parameter) {
  song_data_t songdata;
  while (1) {
    if (xQueueReceive(mp3_data_transfer_queue, &songdata, portMAX_DELAY)) {

      transfer_mp3(songdata);
    }
  }
}

void buttons_interrupt() {

  if (menu_check == 0) {
    volume_control();
  }
  if (menu_check == 1) {
    bass_control();
  }
  if (menu_check == 2) {
    treble_control();
  }

  if (interrupt) {

    if (xSemaphoreTakeFromISR(mp3_control_menu, 0)) {
      if (menu_button) {
        Enter_control_mode();
      } else {
        menu_check = 0;
        mp3__init_lcd_display(0);
      }
    }

    if (xSemaphoreTakeFromISR(mp3_volume_control, 0)) {
      menu_check = 0;
    }
    if (xSemaphoreTakeFromISR(mp3_bass_control, 0)) {
      menu_check = 1;
    }

    if (xSemaphoreTakeFromISR(mp3_treble_control, 0)) {
      menu_check = 2;
    }

    if (xSemaphoreTakeFromISR(mp3_next, 0)) {
      next_song();
    }

    if (xSemaphoreTakeFromISR(mp3_previous, 0)) {
      previous_song();
    }

    if (xSemaphoreTakeFromISR(mp3_pause_play, 0)) {
      if (!currently_playing) {
        play_song_from_home();
      } else {
        if (pause_button) {
          pause_song();
        } else {
          play_song();
        }
      }
    }

    if (xSemaphoreTakeFromISR(song_list_up, 0)) {
      move_up_list();
    }

    if (xSemaphoreTakeFromISR(song_list_down, 0)) {
      move_down_list();
    }
    vTaskDelay(300);
    interrupt = false;
  }
}
static void mp3_control_task(void *p) {
  mp3__init();
  while (1) {

    buttons_interrupt();
    vTaskDelay(50);
  }
}

int main(void) {
  sj2_cli__init();
  mp3_decoder();
  mp3_decoder_init();
  song_list__populate();

  NVIC_EnableIRQ(GPIO_IRQn);

  number_of_songs = song_list__get_item_count();

  songname_queue = xQueueCreate(1, sizeof(songname_t));
  mp3_data_transfer_queue = xQueueCreate(1, sizeof(song_data_t));

  xTaskCreate(mp3_player_task, "Player", 4096 / sizeof(void *), NULL, PRIORITY_HIGH, &mp3_player_handle);
  xTaskCreate(mp3_reader_task, "Reader", 4096 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(mp3_control_task, "Buttons", 4096 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);

  vTaskStartScheduler();

  return 0;
}
