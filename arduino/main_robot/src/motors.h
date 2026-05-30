#ifndef MOTORS_H
#define MOTORS_H

void initMotors();

void driveForward(int pwm);

void moveLeft(int pwm);
void moveRight(int pwm);

#endif
