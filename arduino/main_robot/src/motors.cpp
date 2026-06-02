#include "motors.h"
#include "config.h"

static int clampPwm(int pwm) {
  if (pwm < 0) return 0;
  if (pwm > 255) return 255;
  return pwm;
}

static void printDummyMotorCommand(const char* action) {
  Serial.print("Dummy motor: ");
  Serial.print(action);
  Serial.print(" | forward_pwm=");
  Serial.print(FORWARD_PWM);
  Serial.print(" | side_pwm_a1=");
  Serial.print(SIDE_PWM_A1);
  Serial.print(" | side_pwm_a2=");
  Serial.println(SIDE_PWM_A2);
}

static void stopForwardMotors() {
  analogWrite(B1_FORWARD_PWM_PIN, 0);
  analogWrite(B2_FORWARD_PWM_PIN, 0);
}

static void engageForwardBrakes() {
  digitalWrite(FORWARD_MOTOR_A_BRAKE_PIN, HIGH);
  digitalWrite(FORWARD_MOTOR_B_BRAKE_PIN, HIGH);
}

static void releaseForwardBrakes() {
  digitalWrite(FORWARD_MOTOR_A_BRAKE_PIN, LOW);
  digitalWrite(FORWARD_MOTOR_B_BRAKE_PIN, LOW);
}

static void setForwardMotorDirection() {
  digitalWrite(FORWARD_MOTOR_A_DIRECTION_PIN, HIGH);
  digitalWrite(FORWARD_MOTOR_B_DIRECTION_PIN, LOW);
}

static void stopOneSideHBridge(int in1PwmPin, int in2PwmPin) {
  analogWrite(in1PwmPin, 0);
  analogWrite(in2PwmPin, 0);
  digitalWrite(in1PwmPin, LOW);
  digitalWrite(in2PwmPin, LOW);
}

static void stopSideMotors() {
  stopOneSideHBridge(A1_H_BRIDGE_IN1_PWM_PIN, A1_H_BRIDGE_IN2_PWM_PIN);
  stopOneSideHBridge(A2_H_BRIDGE_IN1_PWM_PIN, A2_H_BRIDGE_IN2_PWM_PIN);
}

// signedPwm > 0: IN1 receives PWM, IN2 LOW.
// signedPwm < 0: IN2 receives PWM, IN1 LOW.
// signedPwm = 0: both LOW, motor coasts/stops.
static void setOneSideHBridge(int in1PwmPin, int in2PwmPin, int signedPwm) {
  if (signedPwm > 0) {
    const int pwm = clampPwm(signedPwm);
    analogWrite(in2PwmPin, 0);
    digitalWrite(in2PwmPin, LOW);
    analogWrite(in1PwmPin, pwm);
  } else if (signedPwm < 0) {
    const int pwm = clampPwm(-signedPwm);
    analogWrite(in1PwmPin, 0);
    digitalWrite(in1PwmPin, LOW);
    analogWrite(in2PwmPin, pwm);
  } else {
    stopOneSideHBridge(in1PwmPin, in2PwmPin);
  }
}

static void setSideMotorsSigned(int a1SignedPwm, int a2SignedPwm) {
  if (SIDE_MOTOR_A1_INVERTED) {
    a1SignedPwm = -a1SignedPwm;
  }
  if (SIDE_MOTOR_A2_INVERTED) {
    a2SignedPwm = -a2SignedPwm;
  }

  setOneSideHBridge(A1_H_BRIDGE_IN1_PWM_PIN, A1_H_BRIDGE_IN2_PWM_PIN, a1SignedPwm);
  setOneSideHBridge(A2_H_BRIDGE_IN1_PWM_PIN, A2_H_BRIDGE_IN2_PWM_PIN, a2SignedPwm);
}

void initMotors() {
  pinMode(B1_FORWARD_PWM_PIN, OUTPUT);
  pinMode(B2_FORWARD_PWM_PIN, OUTPUT);
  pinMode(FORWARD_MOTOR_A_DIRECTION_PIN, OUTPUT);
  pinMode(FORWARD_MOTOR_B_DIRECTION_PIN, OUTPUT);
  pinMode(FORWARD_MOTOR_A_BRAKE_PIN, OUTPUT);
  pinMode(FORWARD_MOTOR_B_BRAKE_PIN, OUTPUT);

  pinMode(A1_H_BRIDGE_IN1_PWM_PIN, OUTPUT);
  pinMode(A1_H_BRIDGE_IN2_PWM_PIN, OUTPUT);
  pinMode(A2_H_BRIDGE_IN1_PWM_PIN, OUTPUT);
  pinMode(A2_H_BRIDGE_IN2_PWM_PIN, OUTPUT);

  stopForwardMotors();
  setForwardMotorDirection();
  engageForwardBrakes();
  stopSideMotors();
}

void stopMotors() {
  if (USE_DUMMY_MOTORS) {
    printDummyMotorCommand("STOP");
    return;
  }

  stopForwardMotors();
  engageForwardBrakes();
  stopSideMotors();
}

void driveForward() {
  if (USE_DUMMY_MOTORS) {
    printDummyMotorCommand("FORWARD");
    return;
  }

  stopSideMotors();
  setForwardMotorDirection();
  releaseForwardBrakes();
  analogWrite(B1_FORWARD_PWM_PIN, clampPwm(FORWARD_PWM));
  analogWrite(B2_FORWARD_PWM_PIN, clampPwm(FORWARD_PWM));
}

void moveLeft() {
  if (USE_DUMMY_MOTORS) {
    printDummyMotorCommand("LEFT");
    return;
  }

  stopForwardMotors();
  engageForwardBrakes();

  // Left movement: both side H-bridges driven in their positive direction.
  // If one motor is physically reversed, set SIDE_MOTOR_Ax_INVERTED = true.
  setSideMotorsSigned(clampPwm(SIDE_PWM_A1), clampPwm(SIDE_PWM_A2));
}

void moveRight() {
  if (USE_DUMMY_MOTORS) {
    printDummyMotorCommand("RIGHT");
    return;
  }

  stopForwardMotors();
  engageForwardBrakes();

  // Right movement: both side H-bridges driven in their negative direction.
  setSideMotorsSigned(-clampPwm(SIDE_PWM_A1), -clampPwm(SIDE_PWM_A2));
}
