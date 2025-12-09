#include "Display.h"
#include "Globals.h"
#include "CnPD.h"

void setupDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();
  delay(100);
}

void displayMsg(const char *msg, uint8_t textSize) {
  Serial.println(msg);
  
  display.clearDisplay();
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(msg);
  display.display();
  
  Serial.flush();
}

// void displayMsg(const char *msg, uint8_t textSize) {
//   unsigned long t = millis();
//   bool first_print = false;
  
//   while (millis() - t <= 3000) {
//     static int16_t scrollOffset = 0;
//     static unsigned long lastScrollTime = 0;

//     display.setTextSize(textSize);
//     display.setTextWrap(false);

//     int16_t x1, y1;
//     uint16_t w, h;
//     display.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);

//     display.clearDisplay();
//     display.setTextColor(SSD1306_WHITE);
//     display.cp437(true);

//     if (w <= SCREEN_WIDTH) {
//       int16_t centerX = (SCREEN_WIDTH - w) / 2;
//       int16_t centerY = (SCREEN_HEIGHT - h) / 2;
//       display.setCursor(centerX, centerY);
//       display.print(msg);
//       display.display();
//       scrollOffset = 0;
//       break;
//     }

//     if (millis() - lastScrollTime > 30) {
//       lastScrollTime = millis();
//       scrollOffset -= 2;

//       // Fix: Cast w to int16_t to avoid signed/unsigned comparison
//       if (scrollOffset <= -((int16_t)w + 10)) {
//         scrollOffset = 0;
//         delay(800);
//       }
//     }

//     int16_t centerY = (SCREEN_HEIGHT - h) / 2;
//     display.setCursor(scrollOffset, centerY);
//     display.print(msg);
//     display.display();
    
//     if (!first_print) {
//       delay(1000);
//       first_print = true;
//     }
//   }

//   Serial.println(msg);
// }