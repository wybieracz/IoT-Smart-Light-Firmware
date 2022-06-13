#include "Time.h"
#include "WiFiConnection.h"
#include "AzIoTClient.h"

void setup() {

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  connectToWiFi();
  initializeTime();
  initializeIoTHubClient();
  (void)initializeMqttClient();
  initializeLightDetector();
  initializeLedController();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_RED, LOW);
    connectToWiFi();
  }
  else checkSession();

  if(autoMode && isEnabled) {
    readLightValue();
    setBrightness();
    delay(10);
  }
}
