/*
  Test button Inputs

  */
#include <Arduino.h>
#include "FastLED.h"
#include <Bounce.h>


#define NUMLEDS 16
#define STRIPPIN 12
#define BRIGHTNESS 48

#define BUTPIN1 0
#define BUTPIN1 1
#define BUTPIN1 2
#define BUTDELAY 20

int pos = 0;
int button1Counter = 0;

// Button objects
Bounce button1 = Bounce(BUTPIN1, BUTDELAY);

CRGB leds[NUMLEDS];
int timeStep = 100;
unsigned long timer = 0;

void setup() {
  FastLED.addLeds<NEOPIXEL, STRIPPIN>(leds, NUMLEDS);
  Serial.begin(115200);
  Serial.print("Starting...");

  // Buttons
  pinMode(BUTPIN1, INPUT_PULLUP);
}

void allBlack() {
  for (size_t i = 0; i < NUMLEDS; i++) {
    leds[i] = CHSV(0,0,0);
  }
}

void drawLEDS() {
  allBlack();
  // pos = (pos + 1) % NUMLEDS;
  leds[pos] = CHSV(0,255,BRIGHTNESS);
  FastLED.show();
}

void input() {
  button1.update();

}
void inputLogic() {
  if (button1.fallingEdge()) {
    button1Counter += 1;
    Serial.print("Button 1 Pressed: ");
    Serial.println(button1Counter);
    pos = (pos + 1) % NUMLEDS;
  }
}


void loop() {

  int pot = analogRead(A1);
  pot = map(pot, 10, 1020, 0, 50);
  // something();


  timeStep = pot;

  input();
  inputLogic();

  if (millis() - timer > timeStep) {
    drawLEDS();
    timer = millis();
  }

}
