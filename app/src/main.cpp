
// Example file name : main.cpp
// Description:
// Test file for SSD1306_OLED library, showing  hello world
// URL: https://github.com/gavinlyonsrepo/SSD1306_OLED_RPI
// *****************************

#include "SSD1306_OLED.h"
#include <bcm2835.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#define myOLEDwidth 128
#define myOLEDheight 64

SSD1306 myOLED(myOLEDwidth, myOLEDheight); // instantiate  an object

// =============== Function prototype ================
void SetupTest(void);
void TestLoop(void);
void EndTest(void);

// ======================= Main ===================
int main(int argc, char **argv) {
  if (!bcm2835_init()) {
    printf("Error 1201: init bcm2835 library\r\n");
    return -1;
  }

  SetupTest();
  TestLoop();
  EndTest();

  return 0;
}
// ======================= End of main  ===================

// ===================== Function Space =====================
void SetupTest() {
  bcm2835_delay(500);
  printf("OLED Begin\r\n");
  myOLED.OLEDbegin();             // initialize the OLED
  myOLED.OLEDFillScreen(0xF0, 0); // splash screen bars
  bcm2835_delay(1500);
}

void EndTest() {
  myOLED.OLEDPowerDown(); // Switch off display
  bcm2835_close();        // Close the library
  printf("OLED End\r\n");
}

void TestLoop() {

  // Define a buffer to cover whole screen
  uint8_t screenBuffer[myOLEDwidth * (myOLEDheight / 8) + 1];
  myOLED.buffer =
      (uint8_t *)&screenBuffer; // set that to library buffer pointer

  myOLED.OLEDclearBuffer(); // Clear active buffer
  myOLED.setTextColor(WHITE);
  myOLED.setTextWrap(false);
  myOLED.setCursor(0, 0);
  myOLED.print("Hello world");

  myOLED.setCursor(5, 1);
  myOLED.print("What\'s up");
  myOLED.OLEDupdate(); // write to active buffer
  delay(500);

  int c_index_y = 0;
  char str[100];
  myOLED.OLEDclearBuffer();
  myOLED.OLEDupdate();
  do {
    bzero(str, sizeof(str));
    myOLED.setCursor(0, c_index_y * 8 % 64);
    fgets(str, sizeof(str), stdin);
    //    scanf("%s",str);
    myOLED.print(str);
    if (c_index_y * 8 % 64 == 0 && c_index_y > 0) {
      printf("SCROLL END DETECTED\n");
      //      myOLED.OLED_StartScrollRight(10, myOLEDwidth);
      myOLED.OLED_StartScrollDiagLeft(0, 8);
      myOLED.OLEDupdate();
      delayMicroseconds(500);
      myOLED.OLED_StopScroll();
    }
    c_index_y++;
    myOLED.OLEDupdate();
  } while (strcmp(str, "exit") != 0);

  int i;
  for (i = 0; i < 100; i++) {
    myOLED.setCursor(0, i * 8 % 64);
    char str[100];
    sprintf(str, "%d", i);
    myOLED.print(str);
    delay(100);
    myOLED.OLEDupdate(); // write to active buffer
    if (i * 8 % 64 == 0) {
      myOLED.OLEDclearBuffer();
    }
  }
}
