#include "src/config.h"
#include "src/sensors.h"
#include "src/motors.h"
#include "src/encoders.h"
#include "src/fsm.h"

unsigned long lastDebugTime = 0;

void initStatusLeds() {
  pinMode(FORWARD_LED_PIN, OUTPUT);
  pinMode(MOVE_LEFT_LED_PIN, OUTPUT);
  pinMode(MOVE_RIGHT_LED_PIN, OUTPUT);
  pinMode(CONFIRM_GAP_LED_PIN, OUTPUT);
  pinMode(STOPPING_LED_PIN, OUTPUT);
}

void updateStatusLeds() {
  const RobotState state = getCurrentState();

  digitalWrite(FORWARD_LED_PIN,     state == STATE_FORWARD ? HIGH : LOW);
  digitalWrite(MOVE_LEFT_LED_PIN,   state == STATE_MOVE_LEFT ? HIGH : LOW);
  digitalWrite(MOVE_RIGHT_LED_PIN,  state == STATE_MOVE_RIGHT ? HIGH : LOW);
  digitalWrite(CONFIRM_GAP_LED_PIN, state == STATE_CONFIRM_GAP ? HIGH : LOW);

  // STOPPING is made deliberately obvious: all LEDs ON.
  if (state == STATE_STOPPING) {
    digitalWrite(FORWARD_LED_PIN, HIGH);
    digitalWrite(MOVE_LEFT_LED_PIN, HIGH);
    digitalWrite(MOVE_RIGHT_LED_PIN, HIGH);
    digitalWrite(CONFIRM_GAP_LED_PIN, HIGH);
    digitalWrite(STOPPING_LED_PIN, HIGH);
  } else {
    digitalWrite(STOPPING_LED_PIN, LOW);
  }
}

void printDebug(const SensorData &s) {
  printSensors(s);
  Serial.print("STATE = ");
  Serial.print(getStateName(getCurrentState()));
  Serial.print(" | side_count = ");
  Serial.print(getSideEncoderCount());
  Serial.print(" | side_travel_mm = ");
  Serial.println(getSideTravelMm());
}

void setup() {
  Serial.begin(9600);

  initStatusLeds();
  updateStatusLeds();

  initSensors();
  initMotors();
  initEncoders();
  initFSM();

  if (ENABLE_DEBUG_PRINT) {
    Serial.println("Maze robot with PWM side H-bridge started.");
    Serial.println("Forward -> side-search -> confirm passable gap -> forward.");
  }
}

void loop() {
  const SensorData sensors = readSensors();

  updateDecision(sensors);
  executeAction();
  updateStatusLeds();

  if (ENABLE_DEBUG_PRINT && millis() - lastDebugTime >= DEBUG_INTERVAL_MS) {
    printDebug(sensors);
    lastDebugTime = millis();
  }
}
