#ifndef FSM_H
#define FSM_H

#include "sensors.h"

enum RobotState {
  STATE_FORWARD,
  STATE_FRONT_BLOCKED,
  STATE_SEARCH_LEFT,
  STATE_SEARCH_RIGHT,
  STATE_CONFIRM_GAP,
  STATE_STOP
};

void initFSM();

void updateDecision(const SensorData &s);
void executeAction();

RobotState getCurrentState();
const char* getStateName(RobotState state);

#endif
