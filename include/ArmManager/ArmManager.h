#ifndef ARM_MANAGER_H
#define ARM_MANAGER_H

#include <Arduino.h>
#include <Servo.h>

class ArmManager {
public:
  ArmManager();
  void begin();
  
  // Hand control
  void closeHand();
  void openHand();
  void stopHand();
  bool isHandClosed();
  bool isHandOpen();
  
  // Lift control
  void liftTo(float heightCm);
  void liftRelative(float deltaCm);
  void stopLift();
  float getCurrentHeight();
  
  // Rotation control
  void rotateToAngle(int angle1, int angle2);
  void rotateToDrop();
  void rotateToNormal();
  
  // Emergency stop
  void stopAll();
  void update();
  void printStatus();
  
  // Encoder interrupt handlers
  void handleHandEncoderA();
  void handleHandEncoderB();
  void handleLiftEncoderA();
  void handleLiftEncoderB();

private:
  struct MotorPins {
    uint8_t in1;
    uint8_t in2;
    uint8_t enable;
    uint8_t encoderA;
    uint8_t encoderB;
  };
  
  MotorPins handPins;
  MotorPins liftPins;
  Servo servo1;
  Servo servo2;
  uint8_t servo1Pin;
  uint8_t servo2Pin;
  
  volatile long handEncoderCount;
  volatile long liftEncoderCount;
  long handTargetCount;
  long liftTargetCount;
  
  enum MotorState { IDLE, MOVING_FORWARD, MOVING_BACKWARD };
  MotorState handState;
  MotorState liftState;
  
  int8_t handCloseDir;
  int8_t liftUpDir;
  float encoderPulsesPerCm;
  long handMaxCount;
  float liftMaxHeight;
  
  int normalAngle1;
  int normalAngle2;
  int dropAngle1;
  int dropAngle2;
  
  void setHandMotor(int speed);
  void setLiftMotor(int speed);
  void updateHandPosition();
  void updateLiftPosition();
  long getHandEncoderCount();
  long getLiftEncoderCount();
  void resetHandEncoder();
  void resetLiftEncoder();
};

#endif