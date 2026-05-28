#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

// =============================
// Reserved pins
// D3, D8, D9, D11, D12, D13 are reserved by the motor shield.
// A0 and A1 are reserved for motor shield current sensing.
// Use A2-A5 for analog sensors.
// =============================

// ----- Sensor pins -----
const int LEFT_SWITCH_PIN  = 2;
const int RIGHT_SWITCH_PIN = 4;

const int FRONT_SENSOR_PIN = A2;
const int REAR_SENSOR_PIN  = A3;   // optional

// ----- Motor shield pins -----
const int MOTOR_A_PWM   = 3;
const int MOTOR_A_DIR   = 12;
const int MOTOR_A_BRAKE = 9;

const int MOTOR_B_PWM   = 11;
const int MOTOR_B_DIR   = 13;
const int MOTOR_B_BRAKE = 8;

// ----- Custom H-bridge pins for side motors -----
const int SIDE_MOTOR_1_IN1 = 5;
const int SIDE_MOTOR_1_IN2 = 6;
const int SIDE_MOTOR_2_IN1 = 7;
const int SIDE_MOTOR_2_IN2 = 10;

// ----- Tunable parameters -----
const int FRONT_BLOCKED_THRESHOLD = 430;
const int FRONT_CLEAR_THRESHOLD   = 350;

const int FORWARD_PWM = 130;
const int SIDE_PWM    = 130;

// Timing
const unsigned long STOP_PAUSE_MS      = 150;
const unsigned long GAP_CONFIRM_MS     = 400;
const unsigned long DEBUG_INTERVAL_MS  = 200;

#endif
