#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "CnPD.h"

void setupMotors();
void forward(uint8_t leftSpeed, uint8_t rightSpeed);
void backward(uint8_t leftSpeed, uint8_t rightSpeed);
void clockwise(uint8_t leftSpeed, uint8_t rightSpeed);
void anticlockwise(uint8_t leftSpeed, uint8_t rightSpeed);
void stop(uint16_t brakeTime = NORMAL_BRAKE_TIME);

#endif