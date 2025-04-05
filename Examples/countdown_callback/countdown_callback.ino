#include "timecontrol.h"

timecontrol countdownTimer;  // No initial timelapse

void finished() {
  Serial.println("Countdown finished!");
}

void setup() {
  Serial.begin(9600);
  Serial.println("5-second countdown");
}

void loop() {
  countdownTimer.countdown(5000, finished);  // Start 5-second countdown
}