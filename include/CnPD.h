#ifndef CnPD_H
#define CnPD_H

#include <Arduino.h>

// Motor Pins
#define LEFT_MOTOR_FORWARD 50
#define LEFT_MOTOR_BACKWARD 51
#define RIGHT_MOTOR_FORWARD 52
#define RIGHT_MOTOR_BACKWARD 53

// Ultrasonic Sensor Pins
const uint8_t US_FRONT[] = {46, 47};
const uint8_t US_LEFT[] = {44, 45};
const uint8_t US_RIGHT[] = {42, 43};

// Color Sensor Pins
#define COLOR_SENSOR_S0 22
#define COLOR_SENSOR_S1 23
#define COLOR_SENSOR_S2 24
#define COLOR_SENSOR_S3 25
#define COLOR_SENSOR_OUT 26
#define COLOR_SENSOR_SCALE 20 

#define COLOR_SENSOR_RED_MIN 0
#define COLOR_SENSOR_RED_MAX 1023
#define COLOR_SENSOR_GREEN_MIN 0
#define COLOR_SENSOR_GREEN_MAX 1023
#define COLOR_SENSOR_BLUE_MIN 0
#define COLOR_SENSOR_BLUE_MAX 1023

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Motor Control
#define NORMAL_BRAKE_TIME 150

#endif