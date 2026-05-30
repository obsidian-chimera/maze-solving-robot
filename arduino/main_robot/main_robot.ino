#include "src/config.h"
#include "src/sensors.h"
#include "src/motors.h"
#include "src/encoders.h"
#include "src/fsm.h"

unsigned long lastDebugTime = 0;

void setup() {
  Serial.begin(9600);

  initSensors();
  initMotors();
  initEncoders();
  initFSM();

  Serial.println("Robot framework started.");
}

void loop() {
  SensorData s = readSensors();

  updateDecision(s);
  executeAction();

  if (ENABLE_DEBUG_PRINT && millis() - lastDebugTime > DEBUG_INTERVAL_MS) {
    printSensors(s);

    Serial.print("ACTION = ");
    Serial.println(getStateName(getCurrentState()));

    lastDebugTime = millis();
  }
}
