#include "src/core/AppContext.hpp"
void setup() {
  AppContext::init();
}

void loop() {
  //ArduinoOTA.handle();
  // Is it alive
  /*unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println("Je suis en vie");
  }*/
}
