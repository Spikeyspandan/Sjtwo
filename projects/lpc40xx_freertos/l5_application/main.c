#include "FreeRTOS.h"
#include "ff.h"
#include "mp3_decoder.h"
#include "mp3_functions.c"
#include "mp3_functions.h"
#include "queue.h"
#include "sj2_cli.h"
#include "song_list.h"
#include "ssp2.h"
#include "stdio.h"
#include "string.h"
#include "task.h"

typedef char songname_t[16];
typedef char song_data_t[512];

QueueHandle_t mp3_data_transfer_queue;
QueueHandle_t songname_queue;

songname_t song_title = {};
songname_t artist_title = {};
songname_t length = {};

songname_t title_list[32];
songname_t artist_list[32];
songname_t time_list[32];

volatile size_t number_of_songs;

volatile uint32_t *gpio__get_iocon(gpio_s gpio);
void pin_configure_adc_channel_as_io_pin() {

  gpio_s gpio = gpio__construct_with_function(GPIO__PORT_0, 25, GPIO__FUNCTION_1);
  gpio_s gpio1 = gpio__construct_with_function(GPIO__PORT_1, 30, GPIO__FUNCTION_3);
  uint32_t *pin_iocon = gpio__get_iocon(gpio);
  uint32_t *pin_iocon1 = gpio__get_iocon(gpio1);

  const uint32_t mode_mask = UINT32_C(3);
  *pin_iocon &= ~(mode_mask << 3);
  *pin_iocon &= ~(1 << 7);

  *pin_iocon1 &= ~(mode_mask << 3);
  *pin_iocon1 &= ~(1 << 7);
}

void volume_control() {
  adc__initialize();

  adc__enable_burst_mode();
  pin_configure_adc_channel_as_io_pin();

  const uint16_t adc_value = adc__get_channel_reading_with_burst_mode(ADC__CHANNEL_2);
  const uint16_t a = adc_value / 40.95;

  if (a == 100) {
    write_register(0x0B, 0x00, 0x00);
  } else if (a == 0) {
    write_register(0x0B, 0xFE, 0xFE);
  }

  else {
    uint16_t value = 0x63 - a;
    write_register(0x0B, value, value);
  }
}

static void open_mp3_file(const char *filename) {
  FIL file;
  UINT br;
  printf("Received song to play; %s\n", filename);
  FRESULT result = f_open(&file, filename, FA_READ);
  if (FR_OK == result) {
    song_data_t buffer = {};

    while (!(f_eof(&file))) {

      if (FR_OK == f_read(&file, buffer, sizeof(buffer), &br)) {

        if (br == 0) {
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
    } else {
      printf("Error while receiving file\n");
    }
  }
}

static void transfer_mp3(song_data_t songdata) {
  for (size_t index = 0; index < sizeof(song_data_t); index++) {
    if (!(Dreq())) {
      vTaskDelay(10);
    }
    send_data(songdata[index]);
  }
}
static void mp3_player_task(void *parameter) {
  song_data_t songdata;
  while (1) {
    if (xQueueReceive(mp3_data_transfer_queue, &songdata, portMAX_DELAY)) {
      volume_control();
      // bass_control();
      transfer_mp3(songdata);
    }
  }
}

static void getmetadata(const char *filename, int index) {
  FIL file;
  UINT br;
  printf("Received song to play; %s\n", filename);
  FRESULT result = f_open(&file, filename, FA_READ);
  result = f_lseek(&file, f_size(&file) - 128);

  songname_t meta_data = {};

  if (FR_OK == f_read(&file, meta_data, sizeof(meta_data), &br)) {
    if (br == 0) {
      printf("Error");
    }
  }
  memset(&song_title[0], 0, sizeof(song_title));
  memset(&artist_title[0], 0, sizeof(song_title));
  memset(length[0], 0, sizeof(song_title));
  int j = 0;
  for (int i = 3; i < 23; i++) {
    song_title[j] = meta_data[i];
    j++;
  }
  j = 0;
  for (int i = 33; i < 53; i++) {
    artist_title[j] = meta_data[i];
    j++;
  }

  strncpy(title_list[index], song_title, sizeof(songname_t));
  strncpy(artist_list[index], artist_title, sizeof(songname_t));
  strncpy(time_list[index], length, sizeof(songname_t));

  f_close(&file);
}

static void populatemetadata(void) {
  for (int i = 0; i < number_of_songs; i++) {
    getmetadata(song_list__get_name_for_item(i), i);
    printf("ASD", title_list[i]);
  }
}

void main(void) {
  sj2_cli__init();
  mp3_decoder();
  mp3_decoder_init();
  song_list__populate();
  number_of_songs = song_list__get_item_count();

  songname_queue = xQueueCreate(1, sizeof(songname_t));
  mp3_data_transfer_queue = xQueueCreate(1, sizeof(song_data_t));
  xTaskCreate(mp3_player_task, "Player", 4096 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(mp3_reader_task, "Reader", 4096 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  vTaskStartScheduler();
}