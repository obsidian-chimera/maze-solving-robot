#include <Arduino.h>
#include "config.h"
#include "fsm.h"
#include "motors.h"

static RobotState currentState = STATE_FORWARD;

static bool preferSearchLeft = true;

static unsigned long stateStartTime = 0;
static unsigned long gapClearStartTime = 0;

static void changeState(RobotState newState) {
  currentState = newState;
  stateStartTime = millis();
}

void initFSM() {
  currentState = STATE_FORWARD;
  preferSearchLeft = true;

  stateStartTime = millis();
  gapClearStartTime = 0;
}

void updateDecision(const SensorData &s) {
  switch (currentState) {

    case STATE_FORWARD:
      if (s.frontBlocked) {
        changeState(STATE_FRONT_BLOCKED);
      }
      break;

    case STATE_FRONT_BLOCKED:
      if (millis() - stateStartTime > STOP_PAUSE_MS) {
        if (preferSearchLeft) {
          changeState(STATE_SEARCH_LEFT);
        } else {
          changeState(STATE_SEARCH_RIGHT);
        }
      }
      break;

    case STATE_SEARCH_LEFT:
      if (s.leftWallHit) {
        preferSearchLeft = false;
        changeState(STATE_SEARCH_RIGHT);
      } else if (s.frontClear) {
        gapClearStartTime = millis();
        changeState(STATE_CONFIRM_GAP);
      }
      break;

    case STATE_SEARCH_RIGHT:
      if (s.rightWallHit) {
        preferSearchLeft = true;
        changeState(STATE_SEARCH_LEFT);
      } else if (s.frontClear) {
        gapClearStartTime = millis();
        changeState(STATE_CONFIRM_GAP);
      }
      break;

    case STATE_CONFIRM_GAP:
      if (s.frontBlocked) {
        if (preferSearchLeft) {
          changeState(STATE_SEARCH_LEFT);
        } else {
          changeState(STATE_SEARCH_RIGHT);
        }
      } else if (s.frontClear && millis() - gapClearStartTime > GAP_CONFIRM_MS) {
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
      driveForward(FORWARD_PWM);
      break;

    case STATE_FRONT_BLOCKED:
      stopAllMotors();
      break;

    case STATE_SEARCH_LEFT:
      moveLeft(SIDE_PWM);
      break;

    case STATE_SEARCH_RIGHT:
      moveRight(SIDE_PWM);
      break;

    case STATE_CONFIRM_GAP:
      if (preferSearchLeft) {
        moveLeft(SIDE_PWM);
      } else {
        moveRight(SIDE_PWM);
      }
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
