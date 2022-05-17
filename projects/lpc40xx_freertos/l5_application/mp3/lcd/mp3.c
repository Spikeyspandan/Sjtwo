#include "mp3.h"
#include "delay.h"
#include "mp3_buttons.h"
#include "volume_treble.h"
#include <string.h>

extern const size_t COL;

char down[8] = {0B00000, 0B00000, 0B00000, 0B10001, 0B11011, 0B01110, 0B00100, 0B00000};
char play[8] = {0B10000, 0B11000, 0B11100, 0B11110, 0B11100, 0B11000, 0B10000, 0B00000};
char pause[8] = {0B10010, 0B10010, 0B10010, 0B10010, 0B10010, 0B10010, 0B10010, 0B00000};
char up[8] = {0B00000, 0B00000, 0B00100, 0B01110, 0B11011, 0B10001, 0B00000, 0B00000};

void command(char send_command) {
  lcd__send_char(0x7C);
  lcd__send_char(send_command);

  // delay_ms(10);
}
void transmit(uint8_t data) { lcd__send_char(data); }
void Special_command(char send_command, char count) {
  for (int i = 0; i < count; i++) {
    transmit(254);          // Send special command character
    transmit(send_command); // Send command code
  }
}

void create_char(char location, char charmap[]) {
  location &= 0x7;
  transmit(0x7C);
  transmit(27 + location);

  for (int i = 0; i < 8; i++) {
    transmit(charmap[i]);
  }
  // delay_ms(50);
}

void writeChar(char location) {
  location &= 0x7;
  command(35 + location);
}
void setCursor(char col, char row) {
  int row_offsets[] = {0x00, 0x40, 0x14, 0x54};

  Special_command(0x80 | (col + row_offsets[row]), 1);
}

void mp3__init_lcd_display(size_t song_index) {
  lcd__clr_screen();
  regular_menu_interrupts();
  size_t song_size = song_list__get_item_count();
  size_t row = 0;
  char temp_buffer[COL * 2]; // just adding padding to keep memory safe
  char row_buffer[COL];
  lcd__clr_screen();
  if (song_size > 0) {                   // at least one song
    while (row < song_size && row < 3) { // 3 cuz only have room to show 3 songs on lcd
      sprintf(temp_buffer, "%u. %s", row + 1, song_list__get_name_for_item(song_index));
      strncpy(row_buffer, temp_buffer, COL); // possibly don't need this func
      lcd__send_row(row_buffer, row);
      song_index++;
      row++;
    }
    setCursor(2, 3);
    writeChar(1);
    setCursor(9, 3);
    writeChar(2);
    writeChar(4);
    setCursor(17, 3);
    writeChar(3);

  } else {
    lcd__send_row("No Songs Found.", ROW_01); // display on the first row.
  }
}

void menu(void) {

  create_char(1, down);
  create_char(2, play);
  create_char(3, up);
  create_char(4, pause);
}

void mp3_controls_display(void) {
  lcd__clr_screen();

  size_t row = 0;
  char temp_buffer[COL * 2]; // just adding padding to keep memory safe
  char row_buffer[COL];
  const char control[3][20] = {"Volume", "Bass", "Treble"};

  // 3 cuz only have room to show 3 songs on lcd
  while (row < 3) {
    sprintf(temp_buffer, "%u. %s", row + 1, control[row]);
    strncpy(row_buffer, temp_buffer, COL); // possibly don't need this func
    lcd__send_row(row_buffer, row);
    row++;
  }

  setCursor(2, 3);
  writeChar(1);
  setCursor(9, 3);
  writeChar(2);
  writeChar(4);
  setCursor(17, 3);
  writeChar(3);
}

void song_playing(int song_number) {
  lcd__clr_screen();
  lcd__send_row("Playing ", ROW_01);
  lcd__send_row(song_list__get_name_for_item(song_number), ROW_02);
  lcd__send_row("Volume :", ROW_03);
}

void song_paused(int song_number) {
  lcd__clr_screen();
  lcd__send_row("Paused ", ROW_01);
  lcd__send_row(song_list__get_name_for_item(song_number), ROW_02);
  lcd__send_row("Volume :", ROW_03);
}

void mp3__init(void) {

  // sl__refresh_list(); // init function for sl = song list
  lcd__init(115200);
  // mp3_pins_init();
  lcd__clr_screen();
  mp3__init_lcd_display(0);

  // mp3_controls_display();

  // while (1) {
  //   volume_control();
  //   delay__ms(10);
  // }
}
