# Main Interfaces
## 1. Motor Interface
```cpp
void driveForward(int pwm);
void driveBackward(int pwm);
void stopForwardMotor();

void moveLeft(int pwm);
void moveRight(int pwm);
void stopSideMotor();

void stopAllMotors();
```

## 2. Sensor Interface
```cpp
SensorData readSensors();
void printSensors(const SensorData &s);
```

## 3. FSM Interface
```cpp
void updateDecision(const SensorData &s);
void executeAction();
RobotState getCurrentState();
```

## 4. State Machine
```cpp
enum RobotState {
  STATE_FORWARD,
  STATE_FRONT_BLOCKED,
  STATE_SEARCH_LEFT,
  STATE_SEARCH_RIGHT,
  STATE_CONFIRM_GAP,
  STATE_STOP
};
```

## 5. File Overview
```cpp
config.h     // Set pin number / threshold / speed
sensors.h    // Declare sensor related function and SensorData
sensors.cpp  // Achieve readSensors()
motors.h     // Declare motor related function
motors.cpp   // Achieve driveForward(), stopAllMotors()
fsm.h        // Declare FSM
fsm.cpp      // Achieve updateDecision(), executeAction()
main_robot.ino // Only be responsible of setup() and loop()
```
