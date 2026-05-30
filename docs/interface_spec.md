# Main Interfaces

## 1. Pin and Signal Interface

| Subsystem | Item | Direction | Type | Pin / Value | Code Name / Function | Current Convention |
|---|---|---|---|---|---|---|
| Motor | Left/right steering PWM | Output | PWM | D3 | `MOTOR_A_PWM` | Controls steering strength |
| Motor | Forward drive PWM | Output | PWM | D11 | `MOTOR_B_PWM` | Controls forward drive strength |
| H-bridge | Steering IN1 | Output | Digital | D6 | `SELF_MADE_H_BRIDGE_IN1_PIN` | Left: HIGH, Right: LOW |
| H-bridge | Steering IN2 | Output | Digital | D7 | `SELF_MADE_H_BRIDGE_IN2_PIN` | Left: LOW, Right: HIGH |
| Sensor | Left side switch | Input | Digital | D2 | `LEFT_SIDE_SWITCH_PIN` | Default: hit = HIGH, polarity to be confirmed |
| Sensor | Right side switch | Input | Digital | D4 | `RIGHT_SIDE_SWITCH_PIN` | Default: hit = HIGH, polarity to be confirmed |
| Sensor | Front photodiode | Input | Analog | A2 | `FRONT_PHOTODIODE_PIN` | Default: larger value = closer wall |
| Sensor | Rear photodiode | Input | Analog | A3 | `REAR_PHOTODIODE_PIN` | Connected, currently debug only |
| Sensor | Front IR LED enable | Output | Digital | D10 | `FRONT_IR_LED_ENABLE_PIN` | Optional, currently not controlled by code |
| Encoder | Side movement count | Input | Count | Dummy now | `getSideEncoderCount()` | Real encoder subsystem to replace dummy count |

## 2. Motor Interface

```cpp
void driveForward(int pwm);

void moveLeft(int pwm);
void moveRight(int pwm);
```

| Function | Input | Output / Effect | Notes |
|---|---|---|---|
| `driveForward(int pwm)` | `pwm` speed command | `MOTOR_A_PWM = 0`, `MOTOR_B_PWM = pwm` | Forward movement only; disables steering PWM |
| `moveLeft(int pwm)` | `pwm` steering command | `MOTOR_B_PWM = 0`, IN1 HIGH, IN2 LOW, `MOTOR_A_PWM = pwm` | Steering/side movement left |
| `moveRight(int pwm)` | `pwm` steering command | `MOTOR_B_PWM = 0`, IN1 LOW, IN2 HIGH, `MOTOR_A_PWM = pwm` | Steering/side movement right |

No public stop or backward command is currently part of the framework.

## 3. Sensor Interface

```cpp
SensorData readSensors();
void printSensors(const SensorData &s);
```

| Data Field | Type | Source | Meaning |
|---|---|---|---|
| `frontRaw` | `int` | A2 / dummy | Raw front photodiode value |
| `rearRaw` | `int` | A3 / dummy | Raw rear photodiode value, debug only |
| `frontBlocked` | `bool` | `frontRaw >= FRONT_BLOCKED_THRESHOLD` | Front wall detected |
| `frontClear` | `bool` | `frontRaw <= FRONT_CLEAR_THRESHOLD` | Gap/front clear detected |
| `leftWallHit` | `bool` | D2 / dummy | Left side switch hit |
| `rightWallHit` | `bool` | D4 / dummy | Right side switch hit |

## 4. FSM Interface

```cpp
void updateDecision(const SensorData &s);
void executeAction();
RobotState getCurrentState();
```

| Function | Input | Output / Effect |
|---|---|---|
| `updateDecision(const SensorData &s)` | Latest `SensorData` | Updates `RobotState` |
| `executeAction()` | Current `RobotState` | Calls motor interface function |
| `getCurrentState()` | None | Returns current `RobotState` |
| `getStateName(RobotState state)` | State enum | Returns debug string |

## 5. Encoder Interface

```cpp
void initEncoders();
void resetSideEncoderCount();
long getSideEncoderCount();
float getSideTravelMm();
bool gapEntryDistanceReached();
```

| Function | Input | Output / Effect |
|---|---|---|
| `initEncoders()` | None | Initializes encoder subsystem |
| `resetSideEncoderCount()` | None | Resets side movement count when gap confirmation starts |
| `getSideEncoderCount()` | None | Returns dummy count now; real encoder subsystem should replace this |
| `getSideTravelMm()` | Encoder count constants | Returns `encoderTurns * wheelCircumference + redundancy` |
| `gapEntryDistanceReached()` | None | True when `sideTravel > robotDiagonal / 2` |

Gap-entry constants are currently:

| Constant | Current Value | Meaning |
|---|---:|---|
| `ENCODER_COUNTS_PER_REV` | `20.0` | Encoder counts per wheel revolution, placeholder |
| `WHEEL_CIRCUMFERENCE_MM` | `210.0` | Wheel circumference, placeholder |
| `GAP_ENTRY_REDUNDANCY_MM` | `10.0` | Extra safety distance |
| `ROBOT_DIAGONAL_MM` | `283.0` | Approx. 20 cm x 20 cm robot diagonal |

## 6. State Machine

```cpp
enum RobotState {
  STATE_FORWARD,
  STATE_MOVE_LEFT,
  STATE_MOVE_RIGHT,
  STATE_CONFIRM_GAP
};
```

| State | Condition / Meaning | Action |
|---|---|---|
| `STATE_FORWARD` | Front not blocked | `driveForward(FORWARD_PWM)` |
| `STATE_MOVE_LEFT` | Front blocked and left side clear | `moveLeft(SIDE_PWM)` |
| `STATE_MOVE_RIGHT` | Front blocked and left side hit, or right-side correction | `moveRight(SIDE_PWM)` |
| `STATE_CONFIRM_GAP` | Front clear while moving sideways | Keep side movement until time and encoder-distance conditions pass |

## 7. Dummy Mode Interface

| Switch | Current Value | Effect |
|---|---:|---|
| `USE_DUMMY_MOTORS` | `true` | Motor commands print to Serial instead of writing PWM pins |
| `USE_DUMMY_SENSORS` | `true` | `readSensors()` returns dummy sensor sequence |
| `USE_DUMMY_ENCODERS` | `true` | Encoder count increases by `DUMMY_ENCODER_COUNTS_PER_READ` |
| `ENABLE_DEBUG_PRINT` | `true` | Serial Monitor prints sensor/action debug output |

## 8. File Overview

```cpp
config.h     // Set pin number / threshold / speed
sensors.h    // Declare sensor related function and SensorData
sensors.cpp  // Achieve readSensors()
motors.h     // Declare motor related function
motors.cpp   // Achieve driveForward(), moveLeft(), moveRight()
encoders.h   // Declare encoder related function
encoders.cpp // Achieve dummy/real encoder distance check for gap entry
fsm.h        // Declare FSM
fsm.cpp      // Achieve updateDecision(), executeAction()
main_robot.ino // Only be responsible of setup() and loop()
```
