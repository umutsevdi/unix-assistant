#ifndef __DISPLAY__
#define __DISPLAY__

#include "SSD1306_OLED.h"
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define disp_width 128
#define disp_height 64
#define DISP_WIDTH_NUMBER_OF_CHARACTERS 21

int disp_setup();

void disp_loop(int read_pipe);

void disp_close();

#endif
