#include "Globals.h"
#include "PoluluQTR/Config.h"
#include "CnPD.h"
#include <Wire.h>

// QTR Sensors
QTRSensors qtrMain, qtrEdge, qtrTyre;

// Sensor arrays
uint16_t mainSensorValues[MAIN_SENSOR_COUNT];
uint16_t edgeSensorValues[EDGE_SENSOR_COUNT];
uint16_t tyreSensorValues[TYRE_SENSOR_COUNT];
bool mainQTRDval[MAIN_SENSOR_COUNT];
bool edgeQTRDval[EDGE_SENSOR_COUNT];
bool tyreQTRDval[TYRE_SENSOR_COUNT];

// PID variables
float KP = 0, KD = 0, KI = 0;
int lastError = 0, totalError = 0;
uint8_t leftMinSpeed = 0, rightMinSpeed = 0;
uint8_t leftBaseSpeed = 0, rightBaseSpeed = 0;
uint8_t leftMaxSpeed = 0, rightMaxSpeed = 0;

// Task Manager
TaskManager taskManager;

// OLED Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);