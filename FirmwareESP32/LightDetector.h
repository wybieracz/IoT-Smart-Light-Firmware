#ifndef LIGHTDETECTOR_H
#define LIGHTDETECTOR_H

#include <Arduino.h>

#define LIGHT_DETECTOR 34
#define PRECISION 40

extern float lightValue;
extern bool autoMode, isLightChanged;


void initializeLightDetector();
void readLightValue();

#endif //LIGHTDETECTOR_H