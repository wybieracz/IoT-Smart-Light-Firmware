#include "LedController.h"

int R = 0;
int G = 0;
int B = 0;
bool isEnabled = true;
bool isChanged = false;

Adafruit_NeoPixel pixels(RGB_LEDS, DIN, NEO_GRB + NEO_KHZ800);

void initializeLedController() {

    pixels.begin();

    R = 128;
    G = 128;
    B = 128;
    isEnabled = true;
    isChanged = true;

    writeToLeds();
}

void writeToLeds() {
    
    if(isChanged) {

        for(int i = 0; i < RGB_LEDS; i++) {
            
            pixels.setPixelColor(i, pixels.Color(R, G, B));
            pixels.show();
        }

        isChanged = false;
    }
    
}

void turnOnOffLeds(bool turnOn) {

    isEnabled = turnOn;

    for(int i = 0; i < RGB_LEDS; i++) {
        
        turnOn ? pixels.setPixelColor(i, pixels.Color(R, G, B)) : pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        pixels.show();
    }
}

void setBrightness() {

    if(isLightChanged) {

        int realR, realG, realB;
        float normalizedValue;

        isLightChanged = false;

        if(lightValue < 2048) {

        normalizedValue = lightValue / 2048;

        realR = R * normalizedValue;
        realG = G * normalizedValue;
        realB = B * normalizedValue;

        if(realR < 5) realR = 0;
        if(realG < 5) realG = 0;
        if(realB < 5) realB = 0;

        }

        if(lightValue >= 2048) {

            normalizedValue = lightValue / 2048 - 1;

            realR = R + (255 - R) * normalizedValue;
            realG = G + (255 - G) * normalizedValue;
            realB = B + (255 - B) * normalizedValue;
        }

        for(int i = 0; i < RGB_LEDS; i++) {

            pixels.setPixelColor(i, pixels.Color(realR, realG, realB));
            pixels.show();
        }
    }
}
