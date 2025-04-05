#include "timecontrol.h"

timecontrol interruptTimer;  // No timelapse, used for interrupts
const int BUTTON_PIN = 2;    // Interrupt pin 

void countEvent() {
  Serial.print("Event detected, count: ");
  Serial.println(interruptTimer.elapsedCount());
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  interruptTimer.setCallback(countEvent);
  interruptTimer.attachInterrupt(BUTTON_PIN, FALLING);  // Interrupt on falling edge
  Serial.println("Counting with interrupt");
}

void loop() {
  // Nothing in loop, handled by interrupt
}
