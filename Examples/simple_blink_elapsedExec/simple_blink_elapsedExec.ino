#include "timecontrol.h"

timecontrol ledTimer(2000);  // 2-second timer

void toggleLED() {
  bool state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
  Serial.print("LED (elapsedExec): ");
  Serial.println(state ? "Off" : "On");
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Example 3: Blink with elapsedExec");
}

void loop() {
  ledTimer.elapsedExec(toggleLED);  // Runs toggleLED if 2 seconds have passed
}
