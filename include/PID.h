#ifndef PID_H
#define PID_H

#include <Arduino.h>

void setLineFollowPID(char mode);
void lineFollow(uint16_t setPoint = 3500);

#endif