#include "Globals.h"
#include "PoluluQTR/QTR.h"
#include "MotorControls.h"

void setLineFollowPID(char mode) {
  // Slow mode - S
  // Medium mode - M
  // Fast mode - F (Choose this worked for this)

  lastError = 0;
  totalError = 0;

  switch (mode) {
    case 'S':
      KP = 1.0f;
      KD = 1.0f;
      KI = 1.0f;

      leftMinSpeed = 0;
      rightMinSpeed = 0;

      leftBaseSpeed = 80;
      rightBaseSpeed = 80;

      leftMaxSpeed = 100;
      rightMaxSpeed = 100;
      break;

    case 'M':
      KP = 1.0f;
      KD = 1.0f;
      KI = 1.0f;

      leftMinSpeed = 0;
      rightMinSpeed = 0;

      leftBaseSpeed = 150;
      rightBaseSpeed = 150;

      leftMaxSpeed = 200;
      rightMaxSpeed = 200;
      break;

    case 'F':
      KP = 1.0f;
      KD = 8.0f;
      KI = 0.0f;

      leftMinSpeed = 0;
      rightMinSpeed = 0;

      leftBaseSpeed = 220;
      rightBaseSpeed = 220;

      leftMaxSpeed = 255;
      rightMaxSpeed = 255;
      break;
  }
}

void lineFollow(uint16_t setPoint) {
  uint16_t position = readMainQTR();
  int error = position - setPoint;
  int controlSpeed = KP * error + KD * (error - lastError) + KI * totalError;

  uint8_t leftSpeed = constrain(leftBaseSpeed + controlSpeed, leftMinSpeed, leftMaxSpeed);
  uint8_t rightSpeed = constrain(rightBaseSpeed - controlSpeed, rightMinSpeed, rightMaxSpeed);

  forward(leftSpeed, rightSpeed);

  lastError = error;
  totalError += error;
}