#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <QTRSensors.h>
#include <Adafruit_SSD1306.h>
#include "TaskManager.h"
#include "PoluluQTR/Config.h"

extern QTRSensors qtrMain, qtrEdge, qtrTyre;
extern Adafruit_SSD1306 display;
extern TaskManager taskManager;

// Added array sizes here
extern uint16_t mainSensorValues[MAIN_SENSOR_COUNT];
extern uint16_t edgeSensorValues[EDGE_SENSOR_COUNT];
extern uint16_t tyreSensorValues[TYRE_SENSOR_COUNT];
extern bool mainQTRDval[MAIN_SENSOR_COUNT];
extern bool edgeQTRDval[EDGE_SENSOR_COUNT];
extern bool tyreQTRDval[TYRE_SENSOR_COUNT];

extern float KP, KD, KI;
extern int lastError, totalError;
extern uint8_t leftMinSpeed, rightMinSpeed;
extern uint8_t leftBaseSpeed, rightBaseSpeed;
extern uint8_t leftMaxSpeed, rightMaxSpeed;

#endif