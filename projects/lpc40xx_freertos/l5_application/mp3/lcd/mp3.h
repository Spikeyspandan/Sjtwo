#pragma once

#include "lcd_driver.h"
#include "song_list.h"

void mp3__init(void);
void song_playing(int song_number);
void setCursor(char col, char row);