#include "timecontrol.h"

// Define two tasks
timecontrol fastTask(500);   // Fast task: blinks LED every 500 ms
timecontrol slowTask(3000);  // Slow task: message every 3 seconds

// Callback for fast task
void blinkLED() {
  bool state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
  Serial.print("Fast Task - LED: ");
  Serial.println(state ? "Off" : "On");
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  // Configure built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Set callback for fast task
  fastTask.setCallback(blinkLED);

  // Start both tasks
  fastTask.resume();
  slowTask.resume();

  Serial.println("Two tasks started:");
  Serial.println("- Fast Task: Blinks LED every 500 ms (with callback)");
  Serial.println("- Slow Task: Message every 3000 ms (no callback)");
}

void loop() {
  // Fast task: triggers callback automatically
  fastTask.elapsed();

  // Slow task: handled manually without callback
  if (slowTask.elapsed()) {
    Serial.println("Slow Task: 2 seconds have passed");
  }

  // Optional: Show status every 5 seconds
  static timecontrol statusTimer(5000);
  if (statusTimer.elapsed()) {
    Serial.println("--- Status ---");
    Serial.print("Fast Task - Remaining time: ");
    Serial.print(fastTask.remainingTime());
    Serial.println(" ms");
    Serial.print("Slow Task - Remaining time: ");
    Serial.print(slowTask.remainingTime());
    Serial.println(" ms");
  }
}