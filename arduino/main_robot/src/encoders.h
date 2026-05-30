#ifndef ENCODERS_H
#define ENCODERS_H

void initEncoders();
void resetSideEncoderCount();

long getSideEncoderCount();
float getSideTravelMm();
bool gapEntryDistanceReached();

#endif
