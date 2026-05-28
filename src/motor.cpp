#include <Arduino.h>
#include "config.h"
#include "motors.h"

void initMotors() {
  pinMode(MOTOR_A_PWM, OUTPUT);
  pinMode(MOTOR_A_DIR, OUTPUT);
  pinMode(MOTOR_A_BRAKE, OUTPUT);

  pinMode(MOTOR_B_PWM, OUTPUT);
  pinMode(MOTOR_B_DIR, OUTPUT);
  pinMode(MOTOR_B_BRAKE, OUTPUT);

  pinMode(SIDE_MOTOR_1_IN1, OUTPUT);
  pinMode(SIDE_MOTOR_1_IN2, OUTPUT);
  pinMode(SIDE_MOTOR_2_IN1, OUTPUT);
  pinMode(SIDE_MOTOR_2_IN2, OUTPUT);

  digitalWrite(MOTOR_A_BRAKE, LOW);
  digitalWrite(MOTOR_B_BRAKE, LOW);

  stopAllMotors();
}

void driveForward(int pwm) {
  stopSideMotor();

  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);

  analogWrite(MOTOR_A_PWM, pwm);
  analogWrite(MOTOR_B_PWM, pwm);
}

void driveBackward(int pwm) {
  stopSideMotor();

  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, LOW);

  analogWrite(MOTOR_A_PWM, pwm);
  analogWrite(MOTOR_B_PWM, pwm);
}

void stopForwardMotor() {
  analogWrite(MOTOR_A_PWM, 0);
  analogWrite(MOTOR_B_PWM, 0);
}

void moveLeft(int pwm) {
  stopForwardMotor();

  analogWrite(SIDE_MOTOR_1_IN1, pwm);
  digitalWrite(SIDE_MOTOR_1_IN2, LOW);

  analogWrite(SIDE_MOTOR_2_IN1, pwm);
  digitalWrite(SIDE_MOTOR_2_IN2, LOW);
}

void moveRight(int pwm) {
  stopForwardMotor();

  digitalWrite(SIDE_MOTOR_1_IN1, LOW);
  analogWrite(SIDE_MOTOR_1_IN2, pwm);

  digitalWrite(SIDE_MOTOR_2_IN1, LOW);
  analogWrite(SIDE_MOTOR_2_IN2, pwm);
}

void stopSideMotor() {
  digitalWrite(SIDE_MOTOR_1_IN1, LOW);
  digitalWrite(SIDE_MOTOR_1_IN2, LOW);
  digitalWrite(SIDE_MOTOR_2_IN1, LOW);
  digitalWrite(SIDE_MOTOR_2_IN2, LOW);
}

void stopAllMotors() {
  stopForwardMotor();
  stopSideMotor();
}
