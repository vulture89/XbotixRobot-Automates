#ifndef ARMCONFIG_H
#define ARMCONFIG_H

// Hand Motor (N20 with Encoder) - L298N Channel A
#define HAND_MOTOR_IN1    2
#define HAND_MOTOR_IN2    3
#define HAND_MOTOR_EN     4
#define HAND_ENCODER_A    18  // Interrupt pin
#define HAND_ENCODER_B    19  // Interrupt pin

// Lift Motor (N20 with Encoder) - L298N Channel B
#define LIFT_MOTOR_IN1    5
#define LIFT_MOTOR_IN2    6
#define LIFT_MOTOR_EN     7
#define LIFT_ENCODER_A    20  // Interrupt pin
#define LIFT_ENCODER_B    21  // Interrupt pin

// Rotation Servos
#define SERVO_1_PIN       9
#define SERVO_2_PIN       10

// ===== DIRECTION CONFIGURATION =====
// Set to 1 for normal direction, -1 to reverse
#define HAND_CLOSE_DIRECTION  1   // 1 = CW closes, -1 = CCW closes
#define LIFT_UP_DIRECTION     1   // 1 = CW lifts up, -1 = CCW lifts up

// ===== CALIBRATION CONSTANTS =====
#define ENCODER_PULSES_PER_CM  100  // Tune this value during testing
#define HAND_MAX_ENCODER_COUNT 400  // Max pulses for fully closed hand
#define LIFT_MAX_HEIGHT_CM     360   // Maximum lift height in cm

#endif