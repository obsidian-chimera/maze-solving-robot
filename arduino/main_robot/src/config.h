#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// =========================================================
// REAL ROBOT / TEST MODE
// =========================================================
// For bench simulation: set all three to true.
// For the real maze run: keep all three false.
const bool USE_DUMMY_MOTORS   = false;
const bool USE_DUMMY_SENSORS  = false;
const bool USE_DUMMY_ENCODERS = false;

const long DUMMY_ENCODER_COUNTS_PER_READ = 2;

// =========================================================
// TARGET BOARD
// =========================================================
// Arduino Mega 2560 expected.
// Reason: this project uses several interrupt pins and PWM pins.

// =========================================================
// STATUS LEDS
// =========================================================
const int FORWARD_LED_PIN     = 30;
const int MOVE_LEFT_LED_PIN   = 14;
const int MOVE_RIGHT_LED_PIN  = 15;
const int CONFIRM_GAP_LED_PIN = 16;
const int STOPPING_LED_PIN    = 17;

// =========================================================
// SENSOR PINS
// =========================================================
const int LEFT_SIDE_SWITCH_PIN  = 7;
const int RIGHT_SIDE_SWITCH_PIN = 4;

const int FRONT_PHOTODIODE_PIN = A2;
const int REAR_PHOTODIODE_PIN  = A3;

// If your side switches are wired to 5 V when pressed, leave this true.
// If you use INPUT_PULLUP and the switch connects to GND when pressed, set false.
const bool SIDE_SWITCH_HIT_IS_HIGH = true;
const bool SIDE_SWITCHES_USE_INTERNAL_PULLUPS = false;

// =========================================================
// SIDE MOTOR CUSTOM H-BRIDGE PINS - NOW PWM CAPABLE
// =========================================================
// Direct-PWM H-bridge control method:
//   one input pin receives PWM while the opposite input is held LOW.
//   reverse direction swaps which input receives PWM.
//
// These four pins are PWM-capable on Arduino Mega 2560.
// Rewire the custom H-bridge inputs to these pins.
const int A1_H_BRIDGE_IN1_PWM_PIN = 44;
const int A1_H_BRIDGE_IN2_PWM_PIN = 45;
const int A2_H_BRIDGE_IN1_PWM_PIN = 46;
const int A2_H_BRIDGE_IN2_PWM_PIN = 10;

// Set either of these true if one side motor moves the wrong way due to
// physical mounting or wiring polarity.
const bool SIDE_MOTOR_A1_INVERTED = false;
const bool SIDE_MOTOR_A2_INVERTED = false;

// =========================================================
// FORWARD MOTOR PINS - Arduino Motor Shield Rev3 style
// =========================================================
const int B1_FORWARD_PWM_PIN = 3;
const int B2_FORWARD_PWM_PIN = 11;

// Validated forward convention: A HIGH, B LOW.
const int FORWARD_MOTOR_A_DIRECTION_PIN = 12;
const int FORWARD_MOTOR_B_DIRECTION_PIN = 13;

// HIGH = brake engaged, LOW = brake released.
const int FORWARD_MOTOR_A_BRAKE_PIN = 9;
const int FORWARD_MOTOR_B_BRAKE_PIN = 8;

// =========================================================
// ENCODERS
// =========================================================
const int ENCODER_COUNT = 4;
// 0 = M1 Front Left, 1 = M2 Rear Left, 2 = M3 Front Right, 3 = M4 Rear Right.
const int ENCODER_A_PINS[ENCODER_COUNT] = {2, 18, 19, 20};
const int ENCODER_B_PINS[ENCODER_COUNT] = {22, 24, 26, 28};

// For gap confirmation, use encoders attached to the motors that actually
// rotate during sideways movement. If the custom H-bridge controls M3/M4,
// these defaults should be sensible. Set SECONDARY to -1 if only using one.
const int SIDE_ENCODER_PRIMARY_INDEX   = 2;
const int SIDE_ENCODER_SECONDARY_INDEX = 3;
const bool SIDE_ENCODER_AVERAGE_TWO    = true;

// =========================================================
// TUNING PARAMETERS
// =========================================================
const int FRONT_BLOCKED_THRESHOLD = 430;
const int FRONT_CLEAR_THRESHOLD   = 350;

// PWM values: 0 = off, 255 = full power.
const int FORWARD_PWM = 130;
const int SIDE_PWM_A1 = 130;
const int SIDE_PWM_A2 = 130;

// Robot geometry / encoder conversion.
const float ENCODER_COUNTS_PER_REV = 182.0;
const float WHEEL_CIRCUMFERENCE_MM = 172.8;

// Gap rejection logic.
// The robot only commits to driving forward if it has side-travelled at least
// this width while the front photodiode still sees clear space.
const float ROBOT_WIDTH_MM          = 200.0;
const float REQUIRED_GAP_MARGIN_MM  = 25.0;
const float MIN_PASSABLE_GAP_MM     = ROBOT_WIDTH_MM + REQUIRED_GAP_MARGIN_MM;

// Compensates for sensor position and small mechanical delays.
const float GAP_ENTRY_REDUNDANCY_MM = 10.0;

// Timing.
const unsigned long GAP_CONFIRM_MS       = 400;
const unsigned long SWITCH_DEBOUNCE_MS   = 40;
const unsigned long MAX_SIDE_SEARCH_MS   = 7000;
const unsigned long MAX_GAP_CONFIRM_MS   = 4000;
const unsigned long DEBUG_INTERVAL_MS    = 200;

const bool ENABLE_DEBUG_PRINT = true;

#endif
