#include "network.h"

int led = 32;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  initNetwork();
}

void loop() {
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
}
