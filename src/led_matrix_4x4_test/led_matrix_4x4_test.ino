#include <Arduino.h>
#include "FastLED.h"
//#include <Bounce.h>

#define NUMLEDS 16
#define STRIPPIN 12
#define BRIGHTNESS 48
int pos = 0;

CRGB leds[NUMLEDS];
int timeStep = 100;
unsigned long timer = 0;

void setup() {
  FastLED.addLeds<NEOPIXEL, STRIPPIN>(leds, NUMLEDS);
}

void allBlack() {
  for (size_t i = 0; i < NUMLEDS; i++) {
    leds[i] = CHSV(0,0,0);
  }
}

void something() {
  allBlack();
  pos+=1;
  leds[pos%NUMLEDS] = CHSV(0,255,BRIGHTNESS);
  FastLED.show();
}


void loop() {

  int pot = analogRead(A1);
  pot = map(pot, 10, 1020, 0, 50);
  // something();


  timeStep = pot;

  if (millis() - timer > timeStep) {
    something();
    timer = millis();
  }

}
