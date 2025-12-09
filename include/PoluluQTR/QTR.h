#ifndef QTR_H
#define QTR_H

#include "PoluluQTR/Config.h"

// QTR Configuration Structure
struct QTRConfig {
  LineMode mode = LineMode::BLACK_LINE;
  uint16_t threshold = QTR_THRESHOLD;
};

void setupQTR();

// External declaration of config instance
extern QTRConfig qtrConfig;

// Template function for processing sensor values
template<size_t N>
inline void processSensorValues(const uint16_t* sensorValues, bool* digitalValues, uint16_t threshold) {
  for (uint8_t i = 0; i < N; i++) {
    digitalValues[i] = sensorValues[i] > threshold;
  }
}

// Function declarations
uint16_t readMainQTR();
void readEdgeQTR();
void readTyreQTR();
void setLineMode(LineMode mode);
void setQTRThreshold(uint16_t threshold);
bool hasCalibrationData();
void saveQTR();
void loadQTR();
void clearQTRCalibration();
void setManualCalibration();
void saveManualCalibration();

#endif