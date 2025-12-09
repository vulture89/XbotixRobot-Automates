#include "ArmManager/ArmManager.h"
#include "ArmManager/ArmConfig.h"

// Global pointer for ISR access
ArmManager* armInstancePtr = nullptr;

// ISR wrappers
void handEncoderA_ISR() {
  if (armInstancePtr) armInstancePtr->handleHandEncoderA();
}

void handEncoderB_ISR() {
  if (armInstancePtr) armInstancePtr->handleHandEncoderB();
}

void liftEncoderA_ISR() {
  if (armInstancePtr) armInstancePtr->handleLiftEncoderA();
}

void liftEncoderB_ISR() {
  if (armInstancePtr) armInstancePtr->handleLiftEncoderB();
}

// Constructor
ArmManager::ArmManager() {
  handEncoderCount = 0;
  liftEncoderCount = 0;
  handTargetCount = 0;
  liftTargetCount = 0;
  handState = IDLE;
  liftState = IDLE;
  
  // Default servo angles (adjust as needed)
  normalAngle1 = 90;
  normalAngle2 = 90;
  dropAngle1 = 180;   // Adjust these for your drop position
  dropAngle2 = 180;
  
  armInstancePtr = this;
}

void ArmManager::begin() {
  // Store pin assignments
  handPins = {HAND_MOTOR_IN1, HAND_MOTOR_IN2, HAND_MOTOR_EN, HAND_ENCODER_A, HAND_ENCODER_B};
  liftPins = {LIFT_MOTOR_IN1, LIFT_MOTOR_IN2, LIFT_MOTOR_EN, LIFT_ENCODER_A, LIFT_ENCODER_B};
  servo1Pin = SERVO_1_PIN;
  servo2Pin = SERVO_2_PIN;
  
  // Store configuration
  handCloseDir = HAND_CLOSE_DIRECTION;
  liftUpDir = LIFT_UP_DIRECTION;
  encoderPulsesPerCm = ENCODER_PULSES_PER_CM;
  handMaxCount = HAND_MAX_ENCODER_COUNT;
  liftMaxHeight = LIFT_MAX_HEIGHT_CM;
  
  // Setup motor pins
  pinMode(handPins.in1, OUTPUT);
  pinMode(handPins.in2, OUTPUT);
  pinMode(handPins.enable, OUTPUT);
  pinMode(liftPins.in1, OUTPUT);
  pinMode(liftPins.in2, OUTPUT);
  pinMode(liftPins.enable, OUTPUT);
  
  // Setup encoder pins
  pinMode(handPins.encoderA, INPUT_PULLUP);
  pinMode(handPins.encoderB, INPUT_PULLUP);
  pinMode(liftPins.encoderA, INPUT_PULLUP);
  pinMode(liftPins.encoderB, INPUT_PULLUP);
  
  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(handPins.encoderA), handEncoderA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(handPins.encoderB), handEncoderB_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(liftPins.encoderA), liftEncoderA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(liftPins.encoderB), liftEncoderB_ISR, CHANGE);
  
  // Setup servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  rotateToNormal();
  
  // Stop all motors
  stopAll();
}

// ===== HAND CONTROL =====

void ArmManager::closeHand() {
  handTargetCount = handMaxCount;
  handState = MOVING_FORWARD;
}

void ArmManager::openHand() {
  handTargetCount = 0;
  handState = MOVING_BACKWARD;
}

void ArmManager::stopHand() {
  setHandMotor(0);
  handState = IDLE;
}

bool ArmManager::isHandClosed() {
  return abs(handEncoderCount - handMaxCount) < 10;
}

bool ArmManager::isHandOpen() {
  return abs(handEncoderCount) < 10;
}

// ===== LIFT CONTROL =====

void ArmManager::liftTo(float heightCm) {
  // Constrain height
  heightCm = constrain(heightCm, 0, liftMaxHeight);
  
  liftTargetCount = (long)(heightCm * encoderPulsesPerCm);
  
  if (liftTargetCount > liftEncoderCount) {
    liftState = MOVING_FORWARD;
  } else if (liftTargetCount < liftEncoderCount) {
    liftState = MOVING_BACKWARD;
  }
}

void ArmManager::liftRelative(float deltaCm) {
  float currentHeight = getCurrentHeight();
  liftTo(currentHeight + deltaCm);
}

void ArmManager::stopLift() {
  setLiftMotor(0);
  liftState = IDLE;
}

float ArmManager::getCurrentHeight() {
  return (float)liftEncoderCount / encoderPulsesPerCm;
}

// ===== ROTATION CONTROL =====

void ArmManager::rotateToAngle(int angle1, int angle2) {
  servo1.write(constrain(angle1, 0, 180));
  servo2.write(constrain(angle2, 0, 180));
}

void ArmManager::rotateToDrop() {
  rotateToAngle(dropAngle1, dropAngle2);
}

void ArmManager::rotateToNormal() {
  rotateToAngle(normalAngle1, normalAngle2);
}

// ===== EMERGENCY STOP =====

void ArmManager::stopAll() {
  stopHand();
  stopLift();
}

// ===== UPDATE FUNCTION =====

void ArmManager::update() {
  updateHandPosition();
  updateLiftPosition();
}

void ArmManager::updateHandPosition() {
  if (handState == IDLE) return;
  
  long error = handTargetCount - handEncoderCount;
  int motorSpeed = 200;  // Base speed (adjust as needed)
  
  if (abs(error) < 5) {
    stopHand();
    return;
  }
  
  // Slow down near target
  if (abs(error) < 50) {
    motorSpeed = map(abs(error), 0, 50, 80, 200);
  }
  
  if (error > 0) {
    setHandMotor(motorSpeed * handCloseDir);
  } else {
    setHandMotor(-motorSpeed * handCloseDir);
  }
}

void ArmManager::updateLiftPosition() {
  if (liftState == IDLE) return;
  
  long error = liftTargetCount - liftEncoderCount;
  int motorSpeed = 200;  // Base speed
  
  if (abs(error) < 5) {
    stopLift();
    return;
  }
  
  // Slow down near target
  if (abs(error) < 30) {
    motorSpeed = map(abs(error), 0, 30, 100, 200);
  }
  
  if (error > 0) {
    setLiftMotor(motorSpeed * liftUpDir);
  } else {
    setLiftMotor(-motorSpeed * liftUpDir);
  }
}

// ===== MOTOR CONTROL HELPERS =====

void ArmManager::setHandMotor(int speed) {
  speed = constrain(speed, -255, 255);
  
  if (speed > 0) {
    digitalWrite(handPins.in1, HIGH);
    digitalWrite(handPins.in2, LOW);
    analogWrite(handPins.enable, speed);
  } else if (speed < 0) {
    digitalWrite(handPins.in1, LOW);
    digitalWrite(handPins.in2, HIGH);
    analogWrite(handPins.enable, -speed);
  } else {
    digitalWrite(handPins.in1, LOW);
    digitalWrite(handPins.in2, LOW);
    analogWrite(handPins.enable, 0);
  }
}

void ArmManager::setLiftMotor(int speed) {
  speed = constrain(speed, -255, 255);
  
  if (speed > 0) {
    digitalWrite(liftPins.in1, HIGH);
    digitalWrite(liftPins.in2, LOW);
    analogWrite(liftPins.enable, speed);
  } else if (speed < 0) {
    digitalWrite(liftPins.in1, LOW);
    digitalWrite(liftPins.in2, HIGH);
    analogWrite(liftPins.enable, -speed);
  } else {
    digitalWrite(liftPins.in1, LOW);
    digitalWrite(liftPins.in2, LOW);
    analogWrite(liftPins.enable, 0);
  }
}

// ===== ENCODER INTERRUPT HANDLERS =====

void ArmManager::handleHandEncoderA() {
  bool a = digitalRead(handPins.encoderA);
  bool b = digitalRead(handPins.encoderB);
  
  if (a == b) {
    handEncoderCount++;
  } else {
    handEncoderCount--;
  }
}

void ArmManager::handleHandEncoderB() {
  bool a = digitalRead(handPins.encoderA);
  bool b = digitalRead(handPins.encoderB);
  
  if (a != b) {
    handEncoderCount++;
  } else {
    handEncoderCount--;
  }
}

void ArmManager::handleLiftEncoderA() {
  bool a = digitalRead(liftPins.encoderA);
  bool b = digitalRead(liftPins.encoderB);
  
  if (a == b) {
    liftEncoderCount++;
  } else {
    liftEncoderCount--;
  }
}

void ArmManager::handleLiftEncoderB() {
  bool a = digitalRead(liftPins.encoderA);
  bool b = digitalRead(liftPins.encoderB);
  
  if (a != b) {
    liftEncoderCount++;
  } else {
    liftEncoderCount--;
  }
}

// ===== ENCODER HELPERS =====

long ArmManager::getHandEncoderCount() {
  noInterrupts();
  long count = handEncoderCount;
  interrupts();
  return count;
}

long ArmManager::getLiftEncoderCount() {
  noInterrupts();
  long count = liftEncoderCount;
  interrupts();
  return count;
}

void ArmManager::resetHandEncoder() {
  noInterrupts();
  handEncoderCount = 0;
  interrupts();
}

void ArmManager::resetLiftEncoder() {
  noInterrupts();
  liftEncoderCount = 0;
  interrupts();
}

// ===== STATUS =====

void ArmManager::printStatus() {
  Serial.print("Hand: ");
  Serial.print(handEncoderCount);
  Serial.print("/");
  Serial.print(handMaxCount);
  Serial.print(" | Lift: ");
  Serial.print(getCurrentHeight());
  Serial.print("cm | State: Hand=");
  Serial.print(handState == IDLE ? "IDLE" : "MOVING");
  Serial.print(" Lift=");
  Serial.println(liftState == IDLE ? "IDLE" : "MOVING");
}