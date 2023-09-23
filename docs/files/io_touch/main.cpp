#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "XIAO2PICO_PIN_MAP.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

int Power = 11;
int PIN  = 12;
#define NUMPIXELS 1

#define CALI_CNT  30
#define LED_PIN   25
#define THRESHOLD 80

volatile bool flag2int = false;

volatile uint32_t start_moment   = 0;
volatile uint32_t trigger_moment = 0;
volatile int32_t  t_diff         = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

enum RGB_color {
  CLEAR, R, G, B
};

enum pinmode {
  Analog, Digital
};

RGB_color flash_r = R;
RGB_color flash_g = CLEAR;
RGB_color flash_b = CLEAR;

int cali_1 = 0;
int cali_2 = 0;
int cali_3 = 0;

int calibration(int pin) {
  int sum = 0;

  for (int i = 0; i < CALI_CNT; i++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH); // turn the LED on (HIGH is the voltage level)
    delayMicroseconds(10);               // wait for a second
    pinMode(pin, INPUT);
    delayMicroseconds(100);
    sum += analogRead(pin);
    delay(50);             // wait for a second
  }
  return (sum/CALI_CNT);
}

void flash_RGB(RGB_color color) {
  switch (color) {
    case R:
      pixels.setPixelColor(0, pixels.Color(220, 0, 0));
      pixels.show();
      break;

    case G:
      pixels.setPixelColor(0, pixels.Color(0, 220, 0));
      pixels.show();
      break;

    case B:
      pixels.setPixelColor(0, pixels.Color(0, 0, 220));
      pixels.show();
      break;

    default:
      pixels.clear();
      pixels.show();
      break;
  }
}

void timber_counter_call_back(uint gpio, uint32_t event_mask) {
  flag2int = true;
  t_diff = time_us_32() - start_moment;
}

RGB_color sense_touch(int pin, int cali, RGB_color color, pinmode mode) {
  if (mode == Analog) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    pinMode(pin, INPUT);
    delayMicroseconds(100);
    if (abs(analogRead(pin) - cali) > THRESHOLD) {
      return color;
    }
    else {
      return CLEAR;
    }
    // delay(5);
  }
  else {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);

    gpio_init(pin);
    start_moment = time_us_32();
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &timber_counter_call_back);
    delay(1); // give it sometime to make sure the interrupt will trigger

    if (t_diff > 5) return color;
    else            return CLEAR;
  }
}

void setup() {
  pixels.begin();
  pinMode(Power,OUTPUT);
  digitalWrite(Power, HIGH);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  delay(3000);

  cali_1 = calibration(D0);
  cali_2 = calibration(D1);
  cali_3 = calibration(D2);

  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);

  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  flash_r = CLEAR;
  flash_g = CLEAR;
  flash_b = CLEAR;

  RGB_color c1 = R;
  RGB_color c2 = G;
  RGB_color c3 = B;

  flash_r = sense_touch(D7, cali_1, c1, Digital); flash_RGB(flash_r);
  flash_g = sense_touch(D8, cali_2, c2, Digital); flash_RGB(flash_g);
  flash_b = sense_touch(D9, cali_3, c3, Digital); flash_RGB(flash_b);

  flash_r = sense_touch(D0, cali_1, c1, Analog); flash_RGB(flash_r);
  flash_g = sense_touch(D1, cali_2, c2, Analog); flash_RGB(flash_g);
  flash_b = sense_touch(D2, cali_3, c3, Analog); flash_RGB(flash_b);
}
