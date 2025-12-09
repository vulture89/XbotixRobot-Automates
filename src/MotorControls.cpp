#include "MotorControls.h"

void setupMotors() {
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  delay(100);
}

void forward(uint8_t leftSpeed, uint8_t rightSpeed) {
  analogWrite(LEFT_MOTOR_FORWARD, leftSpeed);
  analogWrite(LEFT_MOTOR_BACKWARD, 0);
  analogWrite(RIGHT_MOTOR_FORWARD, rightSpeed);
  analogWrite(RIGHT_MOTOR_BACKWARD, 0);
}

void backward(uint8_t leftSpeed, uint8_t rightSpeed) {
  analogWrite(LEFT_MOTOR_FORWARD, 0);
  analogWrite(LEFT_MOTOR_BACKWARD, leftSpeed);
  analogWrite(RIGHT_MOTOR_FORWARD, 0);
  analogWrite(RIGHT_MOTOR_BACKWARD, rightSpeed);
}

void clockwise(uint8_t leftSpeed, uint8_t rightSpeed) {
  analogWrite(LEFT_MOTOR_FORWARD, leftSpeed);
  analogWrite(LEFT_MOTOR_BACKWARD, 0);
  analogWrite(RIGHT_MOTOR_FORWARD, 0);
  analogWrite(RIGHT_MOTOR_BACKWARD, rightSpeed);
}

void anticlockwise(uint8_t leftSpeed, uint8_t rightSpeed) {
  analogWrite(LEFT_MOTOR_FORWARD, 0);
  analogWrite(LEFT_MOTOR_BACKWARD, leftSpeed);
  analogWrite(RIGHT_MOTOR_FORWARD, rightSpeed);
  analogWrite(RIGHT_MOTOR_BACKWARD, 0);
}

void stop(uint16_t brakeTime) {
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);

  delay(brakeTime);
}