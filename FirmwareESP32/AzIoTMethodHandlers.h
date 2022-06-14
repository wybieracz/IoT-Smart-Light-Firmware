#ifndef AZIOTMETHODHANDLERS_H
#define AZIOTMETHODHANDLERS_H

#include <Arduino.h>
#include "LedController.h"

bool toggleLed(int value);
bool setColor(char* ptr);
bool setMode(int value);
bool getStatus(char* ptr);

#endif //AZIOTMETHODHANDLERS_H