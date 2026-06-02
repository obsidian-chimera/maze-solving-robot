#ifndef ENCODERS_H
#define ENCODERS_H

#include <Arduino.h>

void initEncoders();
void resetAllEncoderCounts();
void resetSideEncoderCount();
void selectSideEncoderForLeftMove();
void selectSideEncoderForRightMove();
long getEncoderCount(int encoderIndex);
long getSideEncoderCount();
float getSideTravelMm();
bool gapEntryDistanceReached();

#endif
