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

    digitalWrite(LED_GREEN, HIGH);
    writeToLeds();
}

void writeToLeds() {
    
    if(isChanged) {

        for(int i = 0; i < RGB_LEDS; i++) {
            
            Serial.println(i);
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

void nextStep() {

    if(G < 255) G++;
    else if(R < 255) R++;
    else if(B < 255) B++;
    else {
        R = 0;
        G = 0;
        B = 0;
    }
}

void setBrightness() {

    int realR, realG, realB;
    float normalizedValue = lightValue / 4095 - 1;

    if(lightValue < 2048) {

        realR = R + (255 - R) * normalizedValue;
        realG = G + (255 - G) * normalizedValue;
        realB = B + (255 - B) * normalizedValue;

    } else {

        realR = R * normalizedValue;
        realG = G * normalizedValue;
        realB = B * normalizedValue;
    }

    for(int i = 0; i < RGB_LEDS; i++) {

        Serial.println(i);
        pixels.setPixelColor(i, pixels.Color(R, G, B));
        pixels.show();
    }
}