#include "motors.h"
#include "config.h"

// ----- Dummy motor output -----
// Enable USE_DUMMY_MOTORS in config.h to print motor commands instead of writing pins.
static void printDummyMotorCommand(const char* action, int pwm) {
  Serial.print("Dummy motor: ");
  Serial.print(action);
  Serial.print(" pwm=");
  Serial.println(pwm);
}

void initMotors() {
  pinMode(MOTOR_A_PWM, OUTPUT);
  pinMode(MOTOR_B_PWM, OUTPUT);

  pinMode(SELF_MADE_H_BRIDGE_IN1_PIN, OUTPUT);
  pinMode(SELF_MADE_H_BRIDGE_IN2_PIN, OUTPUT);

  // Set safe startup output values before the FSM starts issuing commands.
  analogWrite(MOTOR_A_PWM, 0);
  analogWrite(MOTOR_B_PWM, 0);
  digitalWrite(SELF_MADE_H_BRIDGE_IN1_PIN, LOW);
  digitalWrite(SELF_MADE_H_BRIDGE_IN2_PIN, LOW);
}

void driveForward(int pwm) {
  if (USE_DUMMY_MOTORS) {
    printDummyMotorCommand("FORWARD", pwm);
    return;
  }

  // Keep movement modes exclusive: forward drive disables steering PWM.
  analogWrite(MOTOR_A_PWM, 0);
  analogWrite(MOTOR_B_PWM, pwm);
}

void moveLeft(int pwm) {
  if (USE_DUMMY_MOTORS) {
    printDummyMotorCommand("LEFT", pwm);
    return;
  }

  // Keep movement modes exclusive: steering disables forward PWM.
  analogWrite(MOTOR_B_PWM, 0);
  digitalWrite(SELF_MADE_H_BRIDGE_IN1_PIN, HIGH);
  digitalWrite(SELF_MADE_H_BRIDGE_IN2_PIN, LOW);
  analogWrite(MOTOR_A_PWM, pwm);
}

void moveRight(int pwm) {
  if (USE_DUMMY_MOTORS) {
    printDummyMotorCommand("RIGHT", pwm);
    return;
  }

  // Keep movement modes exclusive: steering disables forward PWM.
  analogWrite(MOTOR_B_PWM, 0);
  digitalWrite(SELF_MADE_H_BRIDGE_IN1_PIN, LOW);
  digitalWrite(SELF_MADE_H_BRIDGE_IN2_PIN, HIGH);
  analogWrite(MOTOR_A_PWM, pwm);
}
