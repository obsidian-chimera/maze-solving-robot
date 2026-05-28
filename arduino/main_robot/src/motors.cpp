#include "motors.h"
#include "config.h"

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

  stopAllMotors();
}

void driveForward(int pwm) {
  digitalWrite(MOTOR_A_BRAKE, LOW);
  digitalWrite(MOTOR_B_BRAKE, LOW);

  digitalWrite(MOTOR_A_DIR, HIGH);
  digitalWrite(MOTOR_B_DIR, HIGH);

  analogWrite(MOTOR_A_PWM, pwm);
  analogWrite(MOTOR_B_PWM, pwm);
}

void driveBackward(int pwm) {
  digitalWrite(MOTOR_A_BRAKE, LOW);
  digitalWrite(MOTOR_B_BRAKE, LOW);

  digitalWrite(MOTOR_A_DIR, LOW);
  digitalWrite(MOTOR_B_DIR, LOW);

  analogWrite(MOTOR_A_PWM, pwm);
  analogWrite(MOTOR_B_PWM, pwm);
}

void stopForwardMotor() {
  analogWrite(MOTOR_A_PWM, 0);
  analogWrite(MOTOR_B_PWM, 0);

  digitalWrite(MOTOR_A_BRAKE, HIGH);
  digitalWrite(MOTOR_B_BRAKE, HIGH);
}

void moveLeft(int pwm) {
  digitalWrite(SIDE_MOTOR_1_IN1, HIGH);
  digitalWrite(SIDE_MOTOR_1_IN2, LOW);

  digitalWrite(SIDE_MOTOR_2_IN1, HIGH);
  digitalWrite(SIDE_MOTOR_2_IN2, LOW);
}

void moveRight(int pwm) {
  digitalWrite(SIDE_MOTOR_1_IN1, LOW);
  digitalWrite(SIDE_MOTOR_1_IN2, HIGH);

  digitalWrite(SIDE_MOTOR_2_IN1, LOW);
  digitalWrite(SIDE_MOTOR_2_IN2, HIGH);
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