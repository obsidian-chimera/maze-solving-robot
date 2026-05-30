#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

// ----- Dummy subsystem test -----
// Keep these at the top so the framework demo can be enabled quickly.
// Set them to false only when the matching real subsystem is integrated.
const bool USE_DUMMY_MOTORS = true;
const bool USE_DUMMY_SENSORS = true;
const bool USE_DUMMY_ENCODERS = true;

// Dummy encoder count added each time the FSM checks gap-entry distance.
const long DUMMY_ENCODER_COUNTS_PER_READ = 2;

// =============================
// Reserved pins
// D3 and D11 are reserved for PWM motor control.
// Use A2-A5 for analog sensors.
// =============================

// ----- Project pins -----
const int LEFT_SIDE_SWITCH_PIN  = 2;
const int RIGHT_SIDE_SWITCH_PIN = 4;

const int SELF_MADE_H_BRIDGE_IN1_PIN = 6;
const int SELF_MADE_H_BRIDGE_IN2_PIN = 7;

const int FRONT_IR_LED_ENABLE_PIN = 10;  // Optional; currently defined but not controlled by code.
const int FRONT_PHOTODIODE_PIN    = A2;
const int REAR_PHOTODIODE_PIN     = A3;

// ----- Motor PWM pins -----
const int MOTOR_A_PWM = 3;   // Motor A PWM：控制左右转向力度
const int MOTOR_B_PWM = 11;  // Motor B PWM：控制前后移动力度

// ----- Tunable parameters -----
const int FRONT_BLOCKED_THRESHOLD = 430;
const int FRONT_CLEAR_THRESHOLD   = 350;

const int FORWARD_PWM = 130;
const int SIDE_PWM    = 130;

// ----- Gap entry distance -----
// TODO: Replace these placeholder measurements with real robot values.
// Formula: sideTravel = encoderTurns * wheelCircumference + redundancy.
// Enter gap when sideTravel > robotDiagonal / 2.
const float ENCODER_COUNTS_PER_REV = 20.0;
const float WHEEL_CIRCUMFERENCE_MM = 210.0;
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
