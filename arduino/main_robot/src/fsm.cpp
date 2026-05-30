#include <Arduino.h>
#include "fsm.h"
#include "config.h"
#include "motors.h"
#include "encoders.h"

static RobotState currentState = STATE_FORWARD;
static RobotState previousState = STATE_FORWARD;

static unsigned long gapStartTime = 0;

void changeState(RobotState newState) {
  if (newState != currentState) {
    previousState = currentState;
    currentState = newState;

    if (newState == STATE_CONFIRM_GAP) {
      gapStartTime = millis();
      resetSideEncoderCount();
    }

    Serial.print("State changed to: ");
    Serial.println(getStateName(currentState));
  }
}

void initFSM() {
  currentState = STATE_FORWARD;
  previousState = STATE_FORWARD;
  gapStartTime = 0;
}

void updateDecision(const SensorData &s) {
  switch (currentState) {
    case STATE_FORWARD:
      if (s.frontBlocked) {
        if (!s.leftWallHit) {
          changeState(STATE_MOVE_LEFT);
        } 
        else {
          changeState(STATE_MOVE_RIGHT);
        }
      }
      break;

    case STATE_MOVE_LEFT:
      // Front clear has priority over side hit, so the robot enters the gap when detected.
      if (s.frontClear) {
        changeState(STATE_CONFIRM_GAP);
      } 
      // If the robot hits the left side while moving left, move back right.
      else if (s.leftWallHit) {
        changeState(STATE_MOVE_RIGHT);
      }
      break;

    case STATE_MOVE_RIGHT:
      // Front clear has priority over side hit, so the robot enters the gap when detected.
      if (s.frontClear) {
        changeState(STATE_CONFIRM_GAP);
      } 
      // If the robot hits the right side while moving right, move back left.
      else if (s.rightWallHit) {
        changeState(STATE_MOVE_LEFT);
      }
      break;

    case STATE_CONFIRM_GAP:
      // Keep side movement until the front stays clear and encoder distance says the body has entered the gap.
      if (!s.frontClear) {
        if (previousState == STATE_MOVE_LEFT) {
          changeState(STATE_MOVE_LEFT);
        } else {
          changeState(STATE_MOVE_RIGHT);
        }
      } 
      else if (millis() - gapStartTime >= GAP_CONFIRM_MS && gapEntryDistanceReached()) {
        changeState(STATE_FORWARD);
      }
      break;

  }
}

void executeAction() {
  switch (currentState) {
    case STATE_FORWARD:
      driveForward(FORWARD_PWM);
      break;

    case STATE_MOVE_LEFT:
      moveLeft(SIDE_PWM);
      break;

    case STATE_MOVE_RIGHT:
      moveRight(SIDE_PWM);
      break;

    case STATE_CONFIRM_GAP:
      if (previousState == STATE_MOVE_LEFT) {
        moveLeft(SIDE_PWM);
      } else {
        moveRight(SIDE_PWM);
      }
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
    case STATE_MOVE_LEFT:
      return "MOVE_LEFT";
    case STATE_MOVE_RIGHT:
      return "MOVE_RIGHT";
    case STATE_CONFIRM_GAP:
      return "CONFIRM_GAP";
    default:
      return "UNKNOWN";
  }
}
