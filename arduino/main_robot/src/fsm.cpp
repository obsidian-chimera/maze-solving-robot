#include <Arduino.h>
#include "fsm.h"
#include "config.h"
#include "motors.h"

static RobotState currentState = STATE_FORWARD;
static RobotState previousState = STATE_FORWARD;

static SensorData latestSensor;

static unsigned long stateStartTime = 0;
static unsigned long gapStartTime = 0;

static bool searchLeftNext = true;

void changeState(RobotState newState) {
  if (newState != currentState) {
    previousState = currentState;
    currentState = newState;
    stateStartTime = millis();

    if (newState == STATE_CONFIRM_GAP) {
      gapStartTime = millis();
    }

    Serial.print("State changed to: ");
    Serial.println(getStateName(currentState));
  }
}

void initFSM() {
  currentState = STATE_FORWARD;
  previousState = STATE_FORWARD;
  stateStartTime = millis();
  gapStartTime = 0;
  searchLeftNext = true;
}

void updateDecision(const SensorData &s) {
  latestSensor = s;

  switch (currentState) {
    case STATE_FORWARD:
      if (s.frontBlocked) {
        changeState(STATE_FRONT_BLOCKED);
      }
      break;

    case STATE_FRONT_BLOCKED:
      if (millis() - stateStartTime >= STOP_PAUSE_MS) {
        if (s.leftWallHit) {
          changeState(STATE_SEARCH_RIGHT);
        } 
        else if (s.rightWallHit) {
          changeState(STATE_SEARCH_LEFT);
        } 
        else {
          if (searchLeftNext) {
            changeState(STATE_SEARCH_LEFT);
          } else {
            changeState(STATE_SEARCH_RIGHT);
          }

          searchLeftNext = !searchLeftNext;
        }
      }
      break;

    case STATE_SEARCH_LEFT:
      if (s.leftWallHit) {
        changeState(STATE_SEARCH_RIGHT);
      } 
      else if (s.frontClear) {
        changeState(STATE_CONFIRM_GAP);
      }
      break;

    case STATE_SEARCH_RIGHT:
      if (s.rightWallHit) {
        changeState(STATE_SEARCH_LEFT);
      } 
      else if (s.frontClear) {
        changeState(STATE_CONFIRM_GAP);
      }
      break;

    case STATE_CONFIRM_GAP:
      if (!s.frontClear) {
        if (previousState == STATE_SEARCH_LEFT) {
          changeState(STATE_SEARCH_LEFT);
        } else {
          changeState(STATE_SEARCH_RIGHT);
        }
      } 
      else if (millis() - gapStartTime >= GAP_CONFIRM_MS) {
        changeState(STATE_FORWARD);
      }
      break;

    case STATE_STOP:
      break;
  }
}

void executeAction() {
  switch (currentState) {
    case STATE_FORWARD:
      stopSideMotor();
      driveForward(FORWARD_PWM);
      break;

    case STATE_FRONT_BLOCKED:
      stopAllMotors();
      break;

    case STATE_SEARCH_LEFT:
      stopForwardMotor();
      moveLeft(SIDE_PWM);
      break;

    case STATE_SEARCH_RIGHT:
      stopForwardMotor();
      moveRight(SIDE_PWM);
      break;

    case STATE_CONFIRM_GAP:
      stopAllMotors();
      break;

    case STATE_STOP:
      stopAllMotors();
      break;
  }
}

RobotState getCurrentState() {
  return currentState;
}

const char* getStateName(RobotState state) {
  switch (state) {
    case STATE_FORWARD:
      return "FORWARD";
    case STATE_FRONT_BLOCKED:
      return "FRONT_BLOCKED";
    case STATE_SEARCH_LEFT:
      return "SEARCH_LEFT";
    case STATE_SEARCH_RIGHT:
      return "SEARCH_RIGHT";
    case STATE_CONFIRM_GAP:
      return "CONFIRM_GAP";
    case STATE_STOP:
      return "STOP";
    default:
      return "UNKNOWN";
  }
}