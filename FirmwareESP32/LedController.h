#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include "Adafruit_NeoPixel.h"
#include "LightDetector.h"
#include <Arduino.h>

#define DIN 2
#define RGB_LEDS 8
#define LED_GREEN 32

extern int R, G, B;
extern bool isEnabled, isChanged;

extern Adafruit_NeoPixel pixels;

void initializeLedController();
void writeToLeds();
void turnOnOffLeds(bool turnOn);
void setBrightness();

#endif //LEDCONTROLLER_H
