#include "config.h"
#include "sensors.h"
#include "motors.h"
#include "fsm.h"

unsigned long lastDebugTime = 0;

void setup() {
  Serial.begin(9600);

  initSensors();
  initMotors();
  initFSM();

  Serial.println("Robot framework started.");
}

void loop() {
  SensorData s = readSensors();

  updateDecision(s);
  executeAction();

  if (millis() - lastDebugTime > DEBUG_INTERVAL_MS) {
    printSensors(s);

    Serial.print("ACTION = ");
    Serial.println(getStateName(getCurrentState()));

    lastDebugTime = millis();
  }
}
