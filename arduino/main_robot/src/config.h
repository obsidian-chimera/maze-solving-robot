#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

// ----- Dummy subsystem test -----
// Keep these at the top so the framework demo can be enabled quickly.
// Set them to false only when the matching real subsystem is integrated.
const bool USE_DUMMY_MOTORS = false;
const bool USE_DUMMY_SENSORS = true;
const bool USE_DUMMY_ENCODERS = false;

// Dummy encoder count added each time the FSM checks gap-entry distance.
const long DUMMY_ENCODER_COUNTS_PER_READ = 2;

// =============================
// Reserved pins
// Arduino Mega 2560 is the only integration target.
// D3 and D11 are reserved for forward motor PWM.
// D8 and D9 are reserved for Arduino Motor Shield Rev3 brake control.
// D12 and D13 are reserved for Arduino Motor Shield Rev3 direction control.
// D34/D36 and D38/D40 are reserved for the two self-made H-bridges.
// D2/D18/D19/D20 are reserved for encoder channel A interrupts.
// D14-D16 and D30 are reserved for state indicator LEDs.
// Use A2-A5 for analog sensors.
// =============================

// ----- Project pins -----
const int FORWARD_LED_PIN = 30;
const int MOVE_LEFT_LED_PIN = 14;
const int MOVE_RIGHT_LED_PIN = 15;
const int CONFIRM_GAP_LED_PIN = 16;

const int LEFT_SIDE_SWITCH_PIN  = 7;
const int RIGHT_SIDE_SWITCH_PIN = 4;

// ----- Side motor H-bridge pins -----
// M3 and M4 are side motors controlled by two self-made H-bridges.
// M3/M4 run forward for left movement and reverse for right movement.
const int A1_H_BRIDGE_IN1_PIN = 34;
const int A1_H_BRIDGE_IN2_PIN = 36;
const int A2_H_BRIDGE_IN1_PIN = 38;
const int A2_H_BRIDGE_IN2_PIN = 40;

const int FRONT_PHOTODIODE_PIN    = A2;
const int REAR_PHOTODIODE_PIN     = A3;

// ----- Encoder pins -----
// Channel A pins use RISING interrupts. When A rises, the ISR reads channel B to
// decide direction. Encoder readings are used only for distance/gap checks,
// not for motor speed control.
const int ENCODER_COUNT = 4;
// 0 = M1 Front Left, 1 = M2 Rear Left, 2 = M3 Front Right, 3 = M4 Rear Right.
const int ENCODER_A_PINS[ENCODER_COUNT] = {2, 18, 19, 20};
const int ENCODER_B_PINS[ENCODER_COUNT] = {22, 24, 26, 28};

// Use the validated M1 Front Left encoder for both left and right gap-confirm moves.
// Distance checks use abs(count), so the sign can differ by movement direction.
const int LEFT_MOVE_ENCODER_INDEX = 0;
const int RIGHT_MOVE_ENCODER_INDEX = 0;

// ----- Forward motor PWM pins -----
// B1 and B2 only drive the robot forward. They are not connected through H-bridges.
const int B1_FORWARD_PWM_PIN = 3;
const int B2_FORWARD_PWM_PIN = 11;

// ----- Arduino Motor Shield Rev3 direction pins -----
// Role 4 validated forward convention: A HIGH, B LOW.
const int FORWARD_MOTOR_A_DIRECTION_PIN = 12;
const int FORWARD_MOTOR_B_DIRECTION_PIN = 13;

// ----- Arduino Motor Shield Rev3 brake pins -----
// HIGH = brake engaged, LOW = brake released.
const int FORWARD_MOTOR_A_BRAKE_PIN = 9;
const int FORWARD_MOTOR_B_BRAKE_PIN = 8;

// ----- Tunable parameters -----
const int FRONT_BLOCKED_THRESHOLD = 430;
const int FRONT_CLEAR_THRESHOLD   = 350;

const int FORWARD_PWM = 130;
const int SIDE_PWM    = 130;

// ----- Gap entry distance -----
// Formula: sideTravel = encoderTurns * wheelCircumference + redundancy.
// Enter gap when sideTravel > robotDiagonal / 2.
const float ENCODER_COUNTS_PER_REV = 182.0;
const float WHEEL_CIRCUMFERENCE_MM = 172.8; //d=5.5cm=55mm
const float GAP_ENTRY_REDUNDANCY_MM = 10.0;
const float ROBOT_DIAGONAL_MM = 283.0;  // Approx. sqrt(200^2 + 200^2) for a 20 cm x 20 cm robot.

// Timing
// TODO: Confirm gap detection strategy. Default also requires front to stay clear for 400 ms.
const unsigned long GAP_CONFIRM_MS     = 400;
const unsigned long DEBUG_INTERVAL_MS  = 200;

// Debug
// Change this value to false when Serial Monitor output is not needed.
const bool ENABLE_DEBUG_PRINT = true;

#endif
