#include "SSD1306_OLED.h"
#include "proc_util.h"
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#define disp_width 128
#define disp_height 64
#define DISP_WIDTH_NUMBER_OF_CHARACTERS 21

SSD1306 display(disp_width, disp_height); // instantiate  an object

void loop(void);
void display_close(void);

int main(int argc, char **argv) {
  if (!bcm2835_init()) {
    printf("Error 1201: init bcm2835 library\r\n");
    return -1;
  }
  printf("OLED Begin\r\n");
  display.OLEDbegin();             // initialize the OLED
  display.OLEDFillScreen(0xF0, 0); // splash screen bars
  bcm2835_delay(200);

  loop();
  display_close();

  return 0;
}

void loop() {
  // Define a buffer to cover whole screen
  uint8_t screenBuffer[disp_width * (disp_height / 8) + 1];
  display.buffer =
      (uint8_t *)&screenBuffer; // set that to library buffer pointer

  display.OLEDclearBuffer(); // Clear active buffer
  display.setTextColor(WHITE);
  display.setTextWrap(true);

  display.setCursor(0, 0);
  display.print("==== Voice Shell ====");
  display.setCursor(0, 8);
  display.print("== Bash over voice ==");

  // first line is hello world
  int y_index = 2;
  char str[100];
  display.OLEDupdate(); // write to active buffer
  delay(100);

  while (strncmp(str, "exit", 4) != 0) {
    bzero(str, sizeof(str));
    fgets(str, sizeof(str), stdin);
    str[99] = '\0';
    bool is_clear = strncmp(str, "clear", 5) == 0;
    if ((y_index * 8 % 64 == 0 && y_index >= 8) || is_clear) {
      display.OLEDclearBuffer();
      y_index = 0;
      display.OLEDupdate();
      // if clear is written skip displaying
      if (is_clear) {
        continue;
      }
    }
    display.setCursor(0, y_index * 8 % 64);
    char *output = proc_exec(str, PROC_UTIL_BUFFER_SIZE);
    int output_len = strnlen(output, PROC_UTIL_BUFFER_SIZE);
    display.print("$ ");
    display.print(str);
    display.print(output);
    printf("%s \n", output);
    free(output);
    y_index += 2 + output_len / DISP_WIDTH_NUMBER_OF_CHARACTERS;
    display.OLEDupdate();
  }
  printf("exiting\n");
  display.OLEDclearBuffer();
  display.setCursor(0, 0);
  display.print("Bye...");
  display.OLEDupdate();
  delay(1000);
}

void display_close() {
  display.OLEDFillScreen(0xF0, 0); // splash screen bars
  delay(200);
  display.OLEDclearBuffer();
  display.OLEDPowerDown(); // Switch off display
  bcm2835_close();         // Close the library
  printf("OLED End\r\n");
}
