#include "sensors.h"
#include "config.h"

// ----- Dummy sensor values -----
// Change these values when USE_DUMMY_SENSORS is true.
const unsigned long DUMMY_PHASE_MS = 2000;

const int DUMMY_FRONT_CLEAR_RAW = 200;
const int DUMMY_FRONT_BLOCKED_RAW = 500;
const int DUMMY_REAR_RAW = 200;

const bool DUMMY_SIDE_CLEAR = false;
const bool DUMMY_SIDE_HIT = true;

static SensorData readDummySensors() {
  SensorData s;

  // Change the phase duration or the cases below to test different dummy scenarios.
  const unsigned long phase = (millis() / DUMMY_PHASE_MS) % 4;

  // Phase 0 default: front clear, no side wall hit.
  s.frontRaw = DUMMY_FRONT_CLEAR_RAW;
  s.rearRaw = DUMMY_REAR_RAW;
  s.frontBlocked = false;
  s.frontClear = true;
  s.leftWallHit = DUMMY_SIDE_CLEAR;
  s.rightWallHit = DUMMY_SIDE_CLEAR;

  if (phase == 1) {
    // Phase 1: front blocked, left side clear -> FSM should move left.
    s.frontRaw = DUMMY_FRONT_BLOCKED_RAW;
    s.frontBlocked = true;
    s.frontClear = false;
  } else if (phase == 2) {
    // Phase 2: front blocked, left side hit -> FSM should move right.
    s.frontRaw = DUMMY_FRONT_BLOCKED_RAW;
    s.frontBlocked = true;
    s.frontClear = false;
    s.leftWallHit = DUMMY_SIDE_HIT;
  } else if (phase == 3) {
    // Phase 3: front clear again -> FSM should confirm gap, then move forward.
    s.frontRaw = DUMMY_FRONT_CLEAR_RAW;
    s.frontBlocked = false;
    s.frontClear = true;
  }

  return s;
}

void initSensors() {
  // TODO: Sensor subsystem must confirm INPUT vs INPUT_PULLUP wiring.
  // Default: external circuit handles pull-up/pull-down, so use INPUT.
  pinMode(LEFT_SIDE_SWITCH_PIN, INPUT);
  pinMode(RIGHT_SIDE_SWITCH_PIN, INPUT);

  pinMode(FRONT_PHOTODIODE_PIN, INPUT);
  pinMode(REAR_PHOTODIODE_PIN, INPUT);
}

SensorData readSensors() {
  if (USE_DUMMY_SENSORS) {
    return readDummySensors();
  }

  SensorData s;

  s.frontRaw = analogRead(FRONT_PHOTODIODE_PIN);
  // Rear sensor is connected but currently used only for Serial debug output.
  s.rearRaw = analogRead(REAR_PHOTODIODE_PIN);

  // TODO: Confirm photodiode polarity on hardware. Default: larger value = closer wall.
  // Use two thresholds to avoid state jumping near the boundary.
  s.frontBlocked = s.frontRaw >= FRONT_BLOCKED_THRESHOLD;     
  s.frontClear   = s.frontRaw <= FRONT_CLEAR_THRESHOLD;

  // TODO: Confirm switch polarity on hardware. Default: pressed/hit = HIGH.
  s.leftWallHit  = digitalRead(LEFT_SIDE_SWITCH_PIN) == HIGH;
  s.rightWallHit = digitalRead(RIGHT_SIDE_SWITCH_PIN) == HIGH;

  return s;
}

void printSensors(const SensorData &s) {
  Serial.print("Front: ");
  Serial.print(s.frontRaw);

  Serial.print(" | Rear: ");
  Serial.print(s.rearRaw);

  Serial.print(" | FrontBlocked: ");
  Serial.print(s.frontBlocked);

  Serial.print(" | FrontClear: ");
  Serial.print(s.frontClear);

  Serial.print(" | LeftHit: ");
  Serial.print(s.leftWallHit);

  Serial.print(" | RightHit: ");
  Serial.println(s.rightWallHit);
}
