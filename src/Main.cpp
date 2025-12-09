#include "Tasks.h"
#include "Display.h"
#include "PoluluQTR/QTR.h"
#include "MotorControls.h"
#include "Sensors/Ultrasonic.h"

void setup() {
  Serial.begin(9600);
  
  setupDisplay();
  displayMsg("Initializing...");
  setupUltrasonicSensors();
  setupQTR();
  setupMotors();
}

void loop() {
  taskProcessFlow();
}