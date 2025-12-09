#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// QTR Sensor Configuration
const uint8_t MAIN_SENSOR_COUNT = 8;
const uint8_t EDGE_SENSOR_COUNT = 2;
const uint8_t TYRE_SENSOR_COUNT = 2;
const uint16_t QTR_THRESHOLD = 700;
const uint16_t CALIBRATION_COUNT = 100;

// Line following mode enum
enum class LineMode : uint8_t {
  BLACK_LINE,
  WHITE_LINE
};

// EEPROM Memory Map
// Address 0: Calibration valid flag (0xAA = valid)
// Address 1-16: Main QTR minimum values (8 sensors * 1 byte each, divided by 4)
// Address 17-32: Main QTR maximum values (8 sensors * 1 byte each, divided by 4)
// Address 33-36: Edge QTR minimum values (2 sensors * 1 byte each, divided by 4)
// Address 37-40: Edge QTR maximum values (2 sensors * 1 byte each, divided by 4)
// Address 41-44: Tyre QTR minimum values (2 sensors * 1 byte each, divided by 4)
// Address 45-48: Tyre QTR maximum values (2 sensors * 1 byte each, divided by 4)

#define EEPROM_VALID_FLAG 0xAA
#define EEPROM_FLAG_ADDR 0
#define MAIN_MIN_START 1
#define MAIN_MAX_START (MAIN_MIN_START + MAIN_SENSOR_COUNT)
#define EDGE_MIN_START (MAIN_MAX_START + MAIN_SENSOR_COUNT)
#define EDGE_MAX_START (EDGE_MIN_START + EDGE_SENSOR_COUNT)
#define TYRE_MIN_START (EDGE_MAX_START + EDGE_SENSOR_COUNT)
#define TYRE_MAX_START (TYRE_MIN_START + TYRE_SENSOR_COUNT)

// Manual calibration values
#define MANUAL_WHITE_VALUE 100  // Adjust based on your sensors
#define MANUAL_BLACK_VALUE 900  // Adjust based on your sensors

#endif