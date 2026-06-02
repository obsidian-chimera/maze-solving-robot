#include "encoders.h"
#include "config.h"

static volatile long encoderCounts[ENCODER_COUNT] = {0, 0, 0, 0};
static int activeSideEncoderIndex = SIDE_ENCODER_PRIMARY_INDEX;

static long absoluteLong(long value) {
  return value < 0 ? -value : value;
}

static bool validEncoderIndex(int encoderIndex) {
  return encoderIndex >= 0 && encoderIndex < ENCODER_COUNT;
}

static void updateEncoder(int encoderIndex) {
  const int encoderB = digitalRead(ENCODER_B_PINS[encoderIndex]);

  if (encoderB == LOW) {
    encoderCounts[encoderIndex]++;
  } else {
    encoderCounts[encoderIndex]--;
  }
}

static void updateEncoder0() { updateEncoder(0); }
static void updateEncoder1() { updateEncoder(1); }
static void updateEncoder2() { updateEncoder(2); }
static void updateEncoder3() { updateEncoder(3); }

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
  noInterrupts();

  if (validEncoderIndex(SIDE_ENCODER_PRIMARY_INDEX)) {
    encoderCounts[SIDE_ENCODER_PRIMARY_INDEX] = 0;
  }

  if (SIDE_ENCODER_AVERAGE_TWO && validEncoderIndex(SIDE_ENCODER_SECONDARY_INDEX)) {
    encoderCounts[SIDE_ENCODER_SECONDARY_INDEX] = 0;
  }

  if (validEncoderIndex(activeSideEncoderIndex)) {
    encoderCounts[activeSideEncoderIndex] = 0;
  }

  interrupts();
}

void resetAllEncoderCounts() {
  noInterrupts();
  for (int i = 0; i < ENCODER_COUNT; i++) {
    encoderCounts[i] = 0;
  }
  interrupts();
}

void selectSideEncoderForLeftMove() {
  activeSideEncoderIndex = SIDE_ENCODER_PRIMARY_INDEX;
}

void selectSideEncoderForRightMove() {
  activeSideEncoderIndex = SIDE_ENCODER_PRIMARY_INDEX;
}

long getEncoderCount(int encoderIndex) {
  if (!validEncoderIndex(encoderIndex)) {
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
  if (SIDE_ENCODER_AVERAGE_TWO && validEncoderIndex(SIDE_ENCODER_SECONDARY_INDEX)) {
    const long primary = absoluteLong(getEncoderCount(SIDE_ENCODER_PRIMARY_INDEX));
    const long secondary = absoluteLong(getEncoderCount(SIDE_ENCODER_SECONDARY_INDEX));
    return (primary + secondary) / 2;
  }

  return absoluteLong(getEncoderCount(activeSideEncoderIndex));
}

float getSideTravelMm() {
  const long count = getSideEncoderCount();
  const float encoderTurns = count / ENCODER_COUNTS_PER_REV;
  return (encoderTurns * WHEEL_CIRCUMFERENCE_MM) + GAP_ENTRY_REDUNDANCY_MM;
}

bool gapEntryDistanceReached() {
  return getSideTravelMm() >= MIN_PASSABLE_GAP_MM;
}
