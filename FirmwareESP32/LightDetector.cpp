#include "LightDetector.h"

float lightValue = 0.0f;
bool autoMode = false;
bool isLightChanged = false;

void initializeLightDetector() {

    pinMode(LIGHT_DETECTOR, INPUT);
}

void readLightValue() {

    float currentValue = (float)analogRead(LIGHT_DETECTOR);

    if(abs(currentValue - lightValue) > PRECISION) {

        lightValue = currentValue;
        isLightChanged = true;
    }
}