#include "Globals.h"
#include "PoluluQTR/QTR.h"
#include "PoluluQTR/Config.h"
#include <EEPROM.h>

// Define the actual qtrConfig instance
QTRConfig qtrConfig;

void setupQTR() {
  qtrMain.setTypeAnalog();
  qtrEdge.setTypeAnalog();
  qtrTyre.setTypeAnalog();
  qtrMain.setSensorPins((const uint8_t[]){ A15, A14, A13, A12, A11, A10, A9, A8 }, MAIN_SENSOR_COUNT);
  qtrEdge.setSensorPins((const uint8_t[]){ A7, A6 }, EDGE_SENSOR_COUNT);
  qtrTyre.setSensorPins((const uint8_t[]){ A5, A4 }, TYRE_SENSOR_COUNT);
  delay(100);
}

// Main QTR Sensors infront of car
uint16_t readMainQTR() {
  uint16_t position;
  
  if (qtrConfig.mode == LineMode::BLACK_LINE) {
    position = qtrMain.readLineBlack(mainSensorValues);
  } else {
    position = qtrMain.readLineWhite(mainSensorValues);
  }
  
  processSensorValues<MAIN_SENSOR_COUNT>(mainSensorValues, mainQTRDval, qtrConfig.threshold);
  
  return position;
}

// 2 Edge QTR sesnsors placed in front of the car but in edges
void readEdgeQTR() {
  qtrEdge.read(edgeSensorValues);
  processSensorValues<EDGE_SENSOR_COUNT>(edgeSensorValues, edgeQTRDval, qtrConfig.threshold);
}

// 2 QTR sensors placed close to Tyres 
void readTyreQTR() {
  qtrTyre.read(tyreSensorValues);
  processSensorValues<TYRE_SENSOR_COUNT>(tyreSensorValues, tyreQTRDval, qtrConfig.threshold);
}

void setLineMode(LineMode mode) {
  qtrConfig.mode = mode;
}

void setQTRThreshold(uint16_t threshold) {
  qtrConfig.threshold = threshold;
}

// Check if calibration data exists in EEPROM
bool hasCalibrationData() {
  return EEPROM.read(EEPROM_FLAG_ADDR) == EEPROM_VALID_FLAG;
}

// Set manual calibration values for all sensors
void setManualCalibration() {
  // Initialize calibration arrays with a single reading
  qtrMain.calibrate();
  qtrEdge.calibrate();
  qtrTyre.calibrate();
  
  // Set Main QTR manual calibration
  for (uint8_t i = 0; i < MAIN_SENSOR_COUNT; i++) {
    qtrMain.calibrationOn.minimum[i] = MANUAL_WHITE_VALUE;
    qtrMain.calibrationOn.maximum[i] = MANUAL_BLACK_VALUE;
  }
  
  // Set Edge QTR manual calibration
  for (uint8_t i = 0; i < EDGE_SENSOR_COUNT; i++) {
    qtrEdge.calibrationOn.minimum[i] = MANUAL_WHITE_VALUE;
    qtrEdge.calibrationOn.maximum[i] = MANUAL_BLACK_VALUE;
  }
  
  // Set Tyre QTR manual calibration
  for (uint8_t i = 0; i < TYRE_SENSOR_COUNT; i++) {
    qtrTyre.calibrationOn.minimum[i] = MANUAL_WHITE_VALUE;
    qtrTyre.calibrationOn.maximum[i] = MANUAL_BLACK_VALUE;
  }
}

// Store QTR Data for all sensors
void saveQTR() {
  // Calibrate Main QTR
  for (uint16_t i = 0; i < CALIBRATION_COUNT; i++) {
    qtrMain.calibrate();
    delay(10);
  }
  
  // Calibrate Edge QTR
  for (uint16_t i = 0; i < CALIBRATION_COUNT; i++) {
    qtrEdge.calibrate();
    delay(10);
  }
  
  // Calibrate Tyre QTR
  for (uint16_t i = 0; i < CALIBRATION_COUNT; i++) {
    qtrTyre.calibrate();
    delay(10);
  }

  // Save Main QTR calibration data
  for (uint8_t i = 0; i < MAIN_SENSOR_COUNT; i++) {
    EEPROM.write(MAIN_MIN_START + i, qtrMain.calibrationOn.minimum[i] / 4);
    EEPROM.write(MAIN_MAX_START + i, qtrMain.calibrationOn.maximum[i] / 4);
  }
  
  // Save Edge QTR calibration data
  for (uint8_t i = 0; i < EDGE_SENSOR_COUNT; i++) {
    EEPROM.write(EDGE_MIN_START + i, qtrEdge.calibrationOn.minimum[i] / 4);
    EEPROM.write(EDGE_MAX_START + i, qtrEdge.calibrationOn.maximum[i] / 4);
  }
  
  // Save Tyre QTR calibration data
  for (uint8_t i = 0; i < TYRE_SENSOR_COUNT; i++) {
    EEPROM.write(TYRE_MIN_START + i, qtrTyre.calibrationOn.minimum[i] / 4);
    EEPROM.write(TYRE_MAX_START + i, qtrTyre.calibrationOn.maximum[i] / 4);
  }
  
  // Mark calibration as valid
  EEPROM.write(EEPROM_FLAG_ADDR, EEPROM_VALID_FLAG);
}

// Save manual calibration to EEPROM
void saveManualCalibration() {
  // Set manual values
  setManualCalibration();
  
  // Save Main QTR calibration data
  for (uint8_t i = 0; i < MAIN_SENSOR_COUNT; i++) {
    EEPROM.write(MAIN_MIN_START + i, qtrMain.calibrationOn.minimum[i] / 4);
    EEPROM.write(MAIN_MAX_START + i, qtrMain.calibrationOn.maximum[i] / 4);
  }
  
  // Save Edge QTR calibration data
  for (uint8_t i = 0; i < EDGE_SENSOR_COUNT; i++) {
    EEPROM.write(EDGE_MIN_START + i, qtrEdge.calibrationOn.minimum[i] / 4);
    EEPROM.write(EDGE_MAX_START + i, qtrEdge.calibrationOn.maximum[i] / 4);
  }
  
  // Save Tyre QTR calibration data
  for (uint8_t i = 0; i < TYRE_SENSOR_COUNT; i++) {
    EEPROM.write(TYRE_MIN_START + i, qtrTyre.calibrationOn.minimum[i] / 4);
    EEPROM.write(TYRE_MAX_START + i, qtrTyre.calibrationOn.maximum[i] / 4);
  }
  
  // Mark calibration as valid
  EEPROM.write(EEPROM_FLAG_ADDR, EEPROM_VALID_FLAG);
}

// Load QTR Data for all sensors from EEPROM
void loadQTR() {
  // Initialize calibration arrays with a single reading
  qtrMain.calibrate();
  qtrEdge.calibrate();
  qtrTyre.calibrate();
  
  // Load Main QTR calibration data
  for (uint8_t i = 0; i < MAIN_SENSOR_COUNT; i++) {
    qtrMain.calibrationOn.minimum[i] = 4 * EEPROM.read(MAIN_MIN_START + i);
    qtrMain.calibrationOn.maximum[i] = 4 * EEPROM.read(MAIN_MAX_START + i);
  }
  
  // Load Edge QTR calibration data
  for (uint8_t i = 0; i < EDGE_SENSOR_COUNT; i++) {
    qtrEdge.calibrationOn.minimum[i] = 4 * EEPROM.read(EDGE_MIN_START + i);
    qtrEdge.calibrationOn.maximum[i] = 4 * EEPROM.read(EDGE_MAX_START + i);
  }
  
  // Load Tyre QTR calibration data
  for (uint8_t i = 0; i < TYRE_SENSOR_COUNT; i++) {
    qtrTyre.calibrationOn.minimum[i] = 4 * EEPROM.read(TYRE_MIN_START + i);
    qtrTyre.calibrationOn.maximum[i] = 4 * EEPROM.read(TYRE_MAX_START + i);
  }
}

// Clear all calibration data from EEPROM
void clearQTRCalibration() {
  // Clear the valid flag
  EEPROM.write(EEPROM_FLAG_ADDR, 0x00);
  
  // Optionally clear all calibration data
  for (uint16_t i = MAIN_MIN_START; i <= TYRE_MAX_START + TYRE_SENSOR_COUNT; i++) {
    EEPROM.write(i, 0x00);
  }
}