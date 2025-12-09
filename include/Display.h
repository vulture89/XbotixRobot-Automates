#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void setupDisplay();
void displayMsg(const char *msg, uint8_t textSize = 2); 

#endif