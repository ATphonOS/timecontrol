#include "timecontrol.h"

timecontrol ledTimer(1000);  // 1-second timer

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  ledTimer.setRepeatCount(3);  // Limit to 3 repetitions
  Serial.println("3 blinks then pause");
}

void loop() {
  if (ledTimer.elapsed()) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED on");
    delay(100);  // Brief on time
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (ledTimer.elapsedCount() >= 3) {
    ledTimer.pauseAndGetElapsed();  // Pause after 3 events
    Serial.println("Task paused after 3 repetitions");
  }
}
