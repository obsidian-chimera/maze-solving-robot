#include "sensors.h"
#include "config.h"

void initSensors() {
  pinMode(LEFT_SWITCH_PIN, INPUT);
  pinMode(RIGHT_SWITCH_PIN, INPUT);

  pinMode(FRONT_SENSOR_PIN, INPUT);
  pinMode(REAR_SENSOR_PIN, INPUT);
}

SensorData readSensors() {
  SensorData s;

  s.frontRaw = analogRead(FRONT_SENSOR_PIN);
  s.rearRaw = analogRead(REAR_SENSOR_PIN);

  s.frontBlocked = s.frontRaw >= FRONT_BLOCKED_THRESHOLD;
  s.frontClear   = s.frontRaw <= FRONT_CLEAR_THRESHOLD;

  s.leftWallHit  = digitalRead(LEFT_SWITCH_PIN) == HIGH;
  s.rightWallHit = digitalRead(RIGHT_SWITCH_PIN) == HIGH;

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
