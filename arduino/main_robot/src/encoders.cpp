#include "encoders.h"
#include "config.h"

static long sideEncoderCount = 0;

void initEncoders() {
  resetSideEncoderCount();
}

void resetSideEncoderCount() {
  sideEncoderCount = 0;
}

long getSideEncoderCount() {
  if (USE_DUMMY_ENCODERS) {
    sideEncoderCount += DUMMY_ENCODER_COUNTS_PER_READ;
  }

  // TODO: Encoder subsystem should replace this with real side-motion encoder counts.
  return sideEncoderCount;
}

float getSideTravelMm() {
  const float encoderTurns = getSideEncoderCount() / ENCODER_COUNTS_PER_REV;
  return encoderTurns * WHEEL_CIRCUMFERENCE_MM + GAP_ENTRY_REDUNDANCY_MM;
}

bool gapEntryDistanceReached() {
  return getSideTravelMm() > (ROBOT_DIAGONAL_MM / 2.0);
}
