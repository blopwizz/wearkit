#include "FastLED.h"

// Wemos D1 Mini Pro pins
#define BUILTIN_LED 2
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1

#define NUM_LEDS 10                                   // how many leds in your strip ?
#define DATA_PIN D4

CRGB leds[NUM_LEDS];
int delayValue = 30;
int brightness = 0;                                    // how bright the LED is
int fadeAmount = 5;                                    // how many points to fade the LED by
float bpm = 30;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i].setRGB(0, 255, 255);                       // set color
  }
}

void loop() {

  if (Serial.available() > 0) {
    bpm = (float) Serial.read();                              // read the incoming byte:
    Serial.println(bpm);
  }

  delayValue = (int)((60000.0 / 102.0) / bpm);

  brightness = brightness + fadeAmount;               // change brightness for the next loop

  if (brightness <= 0 || brightness >= 255) {         // reverse the direction of the fading when completed
    fadeAmount = -fadeAmount;
  }

  FastLED.setBrightness(int(brightness));
  FastLED.show();
  delay(delayValue);

}
