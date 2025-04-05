#include "timecontrol.h"

timecontrol ledTimer(500);  // 500 ms timer

void blink() {
  bool state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
  Serial.print("LED (callback): ");
  Serial.println(state ? "Off" : "On");
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  ledTimer.setCallback(blink);  // Set callback
  Serial.println("Blink with callback");
}

void loop() {
  ledTimer.elapsed();  // Automatically triggers the callback
}
