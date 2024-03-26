#include "HEX_system.h"

RTCZero rtc;

volatile bool alarmMatched = false;

void alarmMatch(void) {
  alarmMatched = true;
}

void set_idle_level(int level) {
  if (level < 3) {
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk; // Clear SLEEPDEEP bit, we use the idle state.

    if (level == 0) {PM->SLEEP.reg = PM_SLEEP_IDLE_CPU;}
    if (level == 1) {PM->SLEEP.reg = PM_SLEEP_IDLE_AHB;}
    if (level == 2) {PM->SLEEP.reg = PM_SLEEP_IDLE_APB;}
  }
  else {
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  }
  __DSB();
  __WFI();
}

void RTC_setup() {
  // https://github.com/IowaDave/SAMD21-RTC-Clock
  // https://blog.thea.codes/understanding-the-sam-d21-clocks/
  rtc.begin(true, 1);              // initialize RTC: reset starting value, Mode 1 (16-bit counter)
  rtc.setPeriod(PERIOD-1);         // set counter period
  rtc.enableOverflow();            // enable interrupt on overflow
  rtc.attachInterrupt(alarmMatch); // attach interrupt handler

  if (SERIAL_DEBUG) {
    Serial.println("RTC_setup");
  }
}

void serial_setup(void) {
  if (SERIAL_DEBUG) {
    Serial.begin(115200);
    while (!Serial)
      delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }
}