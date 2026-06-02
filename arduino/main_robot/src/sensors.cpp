#include "sensors.h"
#include "config.h"

const unsigned long DUMMY_PHASE_MS = 2000;
const int DUMMY_FRONT_CLEAR_RAW = 200;
const int DUMMY_FRONT_BLOCKED_RAW = 500;
const int DUMMY_REAR_RAW = 200;

struct DebouncedSwitch {
  int pin;
  bool stableHit;
  bool lastRawHit;
  unsigned long lastChangeTime;
};

static DebouncedSwitch leftSwitch  = {LEFT_SIDE_SWITCH_PIN, false, false, 0};
static DebouncedSwitch rightSwitch = {RIGHT_SIDE_SWITCH_PIN, false, false, 0};

// Hysteresis memory for the front sensor.
static bool frontCurrentlyBlocked = false;

static bool rawSwitchHit(int pin) {
  const int raw = digitalRead(pin);
  return SIDE_SWITCH_HIT_IS_HIGH ? (raw == HIGH) : (raw == LOW);
}

static void initDebouncedSwitch(DebouncedSwitch &sw) {
  sw.lastRawHit = rawSwitchHit(sw.pin);
  sw.stableHit = sw.lastRawHit;
  sw.lastChangeTime = millis();
}

static bool updateDebouncedSwitch(DebouncedSwitch &sw) {
  const bool rawHit = rawSwitchHit(sw.pin);

  if (rawHit != sw.lastRawHit) {
    sw.lastRawHit = rawHit;
    sw.lastChangeTime = millis();
  }

  if (millis() - sw.lastChangeTime >= SWITCH_DEBOUNCE_MS) {
    sw.stableHit = rawHit;
  }

  return sw.stableHit;
}

static void updateFrontHysteresis(SensorData &s) {
  if (s.frontRaw >= FRONT_BLOCKED_THRESHOLD) {
    frontCurrentlyBlocked = true;
  } else if (s.frontRaw <= FRONT_CLEAR_THRESHOLD) {
    frontCurrentlyBlocked = false;
  }

  s.frontBlocked = frontCurrentlyBlocked;
  s.frontClear = !frontCurrentlyBlocked;
}

static SensorData readDummySensors() {
  SensorData s;
  const unsigned long phase = (millis() / DUMMY_PHASE_MS) % 5;

  s.frontRaw = DUMMY_FRONT_CLEAR_RAW;
  s.rearRaw = DUMMY_REAR_RAW;
  s.frontBlocked = false;
  s.frontClear = true;
  s.leftWallHit = false;
  s.rightWallHit = false;

  if (phase == 1) {
    // Wall ahead, left available.
    s.frontRaw = DUMMY_FRONT_BLOCKED_RAW;
    s.frontBlocked = true;
    s.frontClear = false;
  } else if (phase == 2) {
    // Small/temporary clear gap.
    s.frontRaw = DUMMY_FRONT_CLEAR_RAW;
    s.frontBlocked = false;
    s.frontClear = true;
  } else if (phase == 3) {
    // Blocked again: the FSM should reject the small gap and keep side-searching.
    s.frontRaw = DUMMY_FRONT_BLOCKED_RAW;
    s.frontBlocked = true;
    s.frontClear = false;
  } else if (phase == 4) {
    // Real clear gap.
    s.frontRaw = DUMMY_FRONT_CLEAR_RAW;
    s.frontBlocked = false;
    s.frontClear = true;
  }

  return s;
}

void initSensors() {
  const int switchMode = SIDE_SWITCHES_USE_INTERNAL_PULLUPS ? INPUT_PULLUP : INPUT;
  pinMode(LEFT_SIDE_SWITCH_PIN, switchMode);
  pinMode(RIGHT_SIDE_SWITCH_PIN, switchMode);

  pinMode(FRONT_PHOTODIODE_PIN, INPUT);
  pinMode(REAR_PHOTODIODE_PIN, INPUT);

  if (!USE_DUMMY_SENSORS) {
    initDebouncedSwitch(leftSwitch);
    initDebouncedSwitch(rightSwitch);
  }
}

SensorData readSensors() {
  if (USE_DUMMY_SENSORS) {
    return readDummySensors();
  }

  SensorData s;
  s.frontRaw = analogRead(FRONT_PHOTODIODE_PIN);
  s.rearRaw = analogRead(REAR_PHOTODIODE_PIN);

  updateFrontHysteresis(s);

  s.leftWallHit = updateDebouncedSwitch(leftSwitch);
  s.rightWallHit = updateDebouncedSwitch(rightSwitch);

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
