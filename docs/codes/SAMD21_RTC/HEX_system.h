#pragma once

#include <Arduino.h>

#include "RTC_FZ.h"

#define SERIAL_DEBUG false

#if !( defined(ARDUINO_SAMD_ZERO) || defined(__SAMD21G18A__))
  #error This code is designed to run on SAMD21/SAMD51 platform! Please check your Tools->Board setting.
#endif

#define PERIOD 10 // sleep period in seconds

enum low_power_t {
  LOW_POWER_IDLE0,
  LOW_POWER_IDLE1,
  LOW_POWER_IDLE2,
  LOW_POWER_STANDBY
};

#define LOW_POWER_LEVEL LOW_POWER_STANDBY

extern volatile bool alarmMatched;

extern void serial_setup(void);

extern void alarmMatch(void);
extern void set_idle_level(int level);

extern void RTC_setup(void);

