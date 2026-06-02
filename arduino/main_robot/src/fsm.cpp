#include <Arduino.h>
#include "fsm.h"
#include "config.h"
#include "motors.h"
#include "encoders.h"

static RobotState currentState = STATE_FORWARD;
static RobotState previousState = STATE_FORWARD;
static RobotState gapDirectionState = STATE_MOVE_LEFT;

static unsigned long stateStartTime = 0;
static unsigned long gapStartTime = 0;

static unsigned long stateElapsedMs() {
  return millis() - stateStartTime;
}

static void changeState(RobotState newState) {
  if (newState == currentState) {
    return;
  }

  previousState = currentState;
  currentState = newState;
  stateStartTime = millis();

  if (newState == STATE_CONFIRM_GAP) {
    gapDirectionState = previousState;

    if (gapDirectionState == STATE_MOVE_LEFT) {
      selectSideEncoderForLeftMove();
    } else {
      selectSideEncoderForRightMove();
    }

    gapStartTime = millis();
    resetSideEncoderCount();
  }

  if (ENABLE_DEBUG_PRINT) {
    Serial.print("State changed to: ");
    Serial.println(getStateName(currentState));
  }
}

static void chooseSideSearchDirection(const SensorData &s) {
  if (!s.leftWallHit) {
    changeState(STATE_MOVE_LEFT);
  } else if (!s.rightWallHit) {
    changeState(STATE_MOVE_RIGHT);
  } else {
    changeState(STATE_STOPPING);
  }
}

static void continueInGapDirection() {
  if (gapDirectionState == STATE_MOVE_LEFT) {
    changeState(STATE_MOVE_LEFT);
  } else {
    changeState(STATE_MOVE_RIGHT);
  }
}

void initFSM() {
  currentState = STATE_FORWARD;
  previousState = STATE_FORWARD;
  gapDirectionState = STATE_MOVE_LEFT;
  stateStartTime = millis();
  gapStartTime = 0;
}

void updateDecision(const SensorData &s) {
  // No available route: front blocked and both side bumpers/switches are hit.
  if (s.frontBlocked && s.leftWallHit && s.rightWallHit) {
    changeState(STATE_STOPPING);
    return;
  }

  switch (currentState) {
    case STATE_FORWARD:
      if (s.frontBlocked) {
        chooseSideSearchDirection(s);
      }
      break;

    case STATE_MOVE_LEFT:
      if (s.frontClear) {
        changeState(STATE_CONFIRM_GAP);
      } else if (s.leftWallHit || stateElapsedMs() >= MAX_SIDE_SEARCH_MS) {
        changeState(STATE_MOVE_RIGHT);
      }
      break;

    case STATE_MOVE_RIGHT:
      if (s.frontClear) {
        changeState(STATE_CONFIRM_GAP);
      } else if (s.rightWallHit || stateElapsedMs() >= MAX_SIDE_SEARCH_MS) {
        changeState(STATE_MOVE_LEFT);
      }
      break;

    case STATE_CONFIRM_GAP:
      // Keep moving sideways through the candidate opening.
      // If the front becomes blocked before enough side travel has happened,
      // the gap is too small for the robot, so ignore it and keep searching.
      if (s.frontBlocked && !gapEntryDistanceReached()) {
        continueInGapDirection();
      }
      // Side contact during confirmation means the current side direction is unsafe.
      else if (gapDirectionState == STATE_MOVE_LEFT && s.leftWallHit) {
        changeState(STATE_MOVE_RIGHT);
      }
      else if (gapDirectionState == STATE_MOVE_RIGHT && s.rightWallHit) {
        changeState(STATE_MOVE_LEFT);
      }
      // Valid gap: front stayed clear long enough AND encoder distance says the
      // open width is at least the minimum passable gap.
      else if ((millis() - gapStartTime >= GAP_CONFIRM_MS) && gapEntryDistanceReached()) {
        changeState(STATE_FORWARD);
      }
      // Safety fallback: if the encoder never increments or a sensor fails, stop.
      else if (stateElapsedMs() >= MAX_GAP_CONFIRM_MS) {
        changeState(STATE_STOPPING);
      }
      break;

    case STATE_STOPPING:
      if (s.frontClear) {
        changeState(STATE_FORWARD);
      } else {
        chooseSideSearchDirection(s);
      }
      break;
  }
}

void executeAction() {
  switch (currentState) {
    case STATE_FORWARD:
      driveForward();
      break;

    case STATE_MOVE_LEFT:
      moveLeft();
      break;

    case STATE_MOVE_RIGHT:
      moveRight();
      break;

    case STATE_CONFIRM_GAP:
      if (gapDirectionState == STATE_MOVE_LEFT) {
        moveLeft();
      } else {
        moveRight();
      }
      break;

    case STATE_STOPPING:
      stopMotors();
      break;
  }
}

RobotState getCurrentState() {
  return currentState;
}

const char* getStateName(RobotState state) {
  switch (state) {
    case STATE_FORWARD:     return "FORWARD";
    case STATE_MOVE_LEFT:   return "MOVE_LEFT";
    case STATE_MOVE_RIGHT:  return "MOVE_RIGHT";
    case STATE_CONFIRM_GAP: return "CONFIRM_GAP";
    case STATE_STOPPING:    return "STOPPING";
    default:                return "UNKNOWN";
  }
}
