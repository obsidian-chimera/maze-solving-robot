#include "encoders.h"
#include "config.h"

static volatile long encoderCounts[ENCODER_COUNT] = {0, 0, 0, 0};
static int activeSideEncoderIndex = LEFT_MOVE_ENCODER_INDEX;

static void updateEncoder(int encoderIndex) {
  const int encoderB = digitalRead(ENCODER_B_PINS[encoderIndex]);

  // Direction convention from the validated Role 4 encoder test:
  // when channel A rises, B LOW means positive travel.
  if (encoderB == LOW) {
    encoderCounts[encoderIndex]++;
  } else {
    encoderCounts[encoderIndex]--;
  }
}

static void updateEncoder0() {
  updateEncoder(0);
}

static void updateEncoder1() {
  updateEncoder(1);
}

static void updateEncoder2() {
  updateEncoder(2);
}

static void updateEncoder3() {
  updateEncoder(3);
}

void initEncoders() {
  for (int i = 0; i < ENCODER_COUNT; i++) {
    pinMode(ENCODER_A_PINS[i], INPUT);
    pinMode(ENCODER_B_PINS[i], INPUT_PULLUP);
  }

  resetAllEncoderCounts();

  if (!USE_DUMMY_ENCODERS) {
    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PINS[0]), updateEncoder0, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PINS[1]), updateEncoder1, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PINS[2]), updateEncoder2, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PINS[3]), updateEncoder3, RISING);
  }
}

void resetSideEncoderCount() {
  if (activeSideEncoderIndex >= 0 && activeSideEncoderIndex < ENCODER_COUNT) {
    noInterrupts();
    encoderCounts[activeSideEncoderIndex] = 0;
    interrupts();
  }
}

void resetAllEncoderCounts() {
  noInterrupts();
  for (int i = 0; i < ENCODER_COUNT; i++) {
    encoderCounts[i] = 0;
  }
  interrupts();
}

void selectSideEncoderForLeftMove() {
  activeSideEncoderIndex = LEFT_MOVE_ENCODER_INDEX;
}

void selectSideEncoderForRightMove() {
  activeSideEncoderIndex = RIGHT_MOVE_ENCODER_INDEX;
}

long getEncoderCount(int encoderIndex) {
  if (encoderIndex < 0 || encoderIndex >= ENCODER_COUNT) {
    return 0;
  }

  if (USE_DUMMY_ENCODERS) {
    noInterrupts();
    encoderCounts[encoderIndex] += DUMMY_ENCODER_COUNTS_PER_READ;
    const long count = encoderCounts[encoderIndex];
    interrupts();
    return count;
  }

  noInterrupts();
  const long count = encoderCounts[encoderIndex];
  interrupts();
  return count;
}

long getSideEncoderCount() {
  return getEncoderCount(activeSideEncoderIndex);
}

float getSideTravelMm() {
  const long count = getSideEncoderCount();
  const long distanceCount = (count < 0) ? -count : count;
  const float encoderTurns = distanceCount / ENCODER_COUNTS_PER_REV;
  return encoderTurns * WHEEL_CIRCUMFERENCE_MM + GAP_ENTRY_REDUNDANCY_MM;
}

bool gapEntryDistanceReached() {
  return getSideTravelMm() > (ROBOT_DIAGONAL_MM / 2.0);
}
