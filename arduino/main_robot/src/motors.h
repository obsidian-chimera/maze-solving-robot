#ifndef MOTORS_H
#define MOTORS_H

void initMotors();

void driveForward(int pwm);
void driveBackward(int pwm);
void stopForwardMotor();

void moveLeft(int pwm);
void moveRight(int pwm);
void stopSideMotor();

void stopAllMotors();

#endif
