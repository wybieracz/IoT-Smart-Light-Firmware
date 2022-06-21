#include "AzIoTMethodHandlers.h"

bool toggleLed(int value) {

    value -= 48;
    
    if(value == 1) {
        turnOnOffLeds(true);
        return true;
    }
    if(value == 0) {
        turnOnOffLeds(false);
        return true;
    }
    return false;
}

bool setColor(char* ptr) {

    int RGB[3] = {0, 0, 0};

    ptr++;

    for(int i = 0; i < 3; i++) {
        RGB[i] += (*ptr - 48) * 100;
        ptr++;
        RGB[i] += (*ptr - 48) * 10;
        ptr++;
        RGB[i] += (*ptr - 48);
        ptr++;
    }

    if((RGB[0] >= 0 && RGB[0] < 256) && (RGB[1] >= 0 && RGB[1] < 256) && (RGB[2] >= 0 && RGB[2] < 256)) {

        R = RGB[0];
        G = RGB[1];
        B = RGB[2];
        isChanged = true;

        if(isEnabled) writeToLeds();

        return true;
    }
    else return false;
}

bool setMode(int value) {

    value -= 48;
    
    if(value == 1) {
        autoMode = true;
        digitalWrite(LED_GREEN, HIGH);
        return true;
    }
    if(value == 0) {
        autoMode = false;
        digitalWrite(LED_GREEN, LOW);
        isChanged = true;
        writeToLeds();
        return true;
    }
    return false;
}

bool getStatus(char* ptr) {

    if(!ptr) return false;

    ptr += 13;
    *ptr = isEnabled + 48;
    ptr += 13;
    *ptr = autoMode + 48;
    ptr += 7;
    *ptr = R / 100 + 48;
    ptr++;
    *ptr = (R % 100) / 10 + 48;
    ptr++;
    *ptr = R % 10 + 48;
    ptr += 8;
    *ptr = G / 100 + 48;
    ptr++;
    *ptr = (G % 100) / 10 + 48;
    ptr++;
    *ptr = G % 10 + 48;
    ptr += 8;
    *ptr = B / 100 + 48;
    ptr++;
    *ptr = (B % 100) / 10 + 48;
    ptr++;
    *ptr = B % 10 + 48;

    return true;
}
