#include "display.h"
#include "proc_util.h"
#include <cstdio>
#include <cstring>

pid_t pid;
SSD1306 display(disp_width, disp_height); // instantiate  an object

int disp_setup(pid_t p) {
  pid = p;
  if (!bcm2835_init()) {
    printf("%i\tError 1201: init bcm2835 library\n", pid);
    return -1;
  }
  printf("%i\tOLED Begin\n", pid);
  display.OLEDbegin();             // initialize the OLED
  display.OLEDFillScreen(0xF0, 0); // splash screen bars
  bcm2835_delay(200);

  return 0;
}

void disp_loop(int read_pipe) {
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
    // read input from pipe
    int len = read(read_pipe, str, sizeof(str));
    if (len <= 0) {
      continue;
    }
    //    str[len] = '\0';
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
    printf("%i\t%s \n", pid, output);
    free(output);
    y_index += 2 + output_len / DISP_WIDTH_NUMBER_OF_CHARACTERS;
    display.OLEDupdate();
  }
  printf("%i\tExiting\n", pid);
  display.OLEDclearBuffer();
  display.setCursor(0, 0);
  display.print("Bye...");
  display.OLEDupdate();
  delay(1000);
}

void disp_close() {
  display.OLEDFillScreen(0xF0, 0); // splash screen bars
  delay(200);
  display.OLEDclearBuffer();
  display.OLEDPowerDown(); // Switch off display
  bcm2835_close();         // Close the library
  printf("%i\tConnection with OLED screen is closed\n", pid);
}
