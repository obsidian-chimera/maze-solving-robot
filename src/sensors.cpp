#include <Arduino.h>
#include "config.h"
#include "sensors.h"

void initSensors() {
  pinMode(LEFT_SWITCH_PIN, INPUT);
  pinMode(RIGHT_SWITCH_PIN, INPUT);
}

SensorData readSensors() {
  SensorData s;

  s.frontRaw = analogRead(FRONT_SENSOR_PIN);
  s.rearRaw = analogRead(REAR_SENSOR_PIN);

  s.leftWallHit = digitalRead(LEFT_SWITCH_PIN);
  s.rightWallHit = digitalRead(RIGHT_SWITCH_PIN);

  s.frontBlocked = s.frontRaw > FRONT_BLOCKED_THRESHOLD;
  s.frontClear = s.frontRaw < FRONT_CLEAR_THRESHOLD;

  return s;
}

void printSensors(const SensorData &s) {
  Serial.print("FRONT = ");
  Serial.print(s.frontRaw);

  Serial.print(", REAR = ");
  Serial.print(s.rearRaw);

  Serial.print(", LEFT = ");
  Serial.print(s.leftWallHit);

  Serial.print(", RIGHT = ");
  Serial.print(s.rightWallHit);

  Serial.print(", ");
}
