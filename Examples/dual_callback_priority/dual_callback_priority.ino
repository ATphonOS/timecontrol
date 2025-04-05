#include "timecontrol.h"

timecontrol timer(1000);  // 1-second timer

void simpleAction() {
  Serial.println("Simple event executed");
}

void showTime(uint32_t elapsedTime) {
  Serial.print("Elapsed time: ");
  Serial.print(elapsedTime);
  Serial.println(" ms");
}

void setup() {
  Serial.begin(9600);
  timer.setCallback(simpleAction);          // Simple callback
  timer.setElapsedCallback(showTime);       // Callback with time
  timer.setPriorityCallback(true);          // Run elapsedCallback first
  Serial.println("Callbacks with elapsed time");
}

void loop() {
  timer.elapsed();  // Triggers both callbacks
}
