#include <Arduino.h>

#include "HEX_system.h"

#define WAIT_TIME 30000

void system_init(void) {
  if (SERIAL_DEBUG) serial_setup();

  RTC_setup();
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  system_init();
}

void loop() {
  if (alarmMatched) {
    alarmMatched = false;

    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

    set_idle_level(LOW_POWER_LEVEL);
  }
}
