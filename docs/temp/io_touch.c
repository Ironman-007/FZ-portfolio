#include <Adafruit_NeoPixel.h>

int Power = 11;
int PIN  = 12;
#define NUMPIXELS 1

#define CALI_CNT  30
#define LED_PIN   25
#define THRESHOLD 100

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

enum RGB_color {
  CLEAR, R, G, B
};

RGB_color flash_r = CLEAR;
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

RGB_color sense_touch(int pin, int cali, RGB_color color) {
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
  delay(50);
}

void setup() {
  pixels.begin();
  pinMode(Power,OUTPUT);
  digitalWrite(Power, HIGH);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  cali_1 = calibration(D0);
  cali_2 = calibration(D1);
  cali_3 = calibration(D2);

  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  flash_r = CLEAR;
  flash_g = CLEAR;
  flash_b = CLEAR;

  RGB_color c1 = R;
  RGB_color c2 = G;
  RGB_color c3 = B;

  flash_r = sense_touch(D0, cali_1, c1); flash_RGB(flash_r);
  flash_g = sense_touch(D1, cali_2, c2); flash_RGB(flash_g);
  flash_b = sense_touch(D2, cali_3, c3); flash_RGB(flash_b);
}
