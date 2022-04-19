#include "AzIoTMethodHandlers.h"

bool toggleLed(int value) {
    
    if(value == 1) {
        digitalWrite(LED_GREEN, HIGH);
        return true;
    }
    if(value == 0) {
        digitalWrite(LED_GREEN, LOW);
        return true;
    }
    return false;
}