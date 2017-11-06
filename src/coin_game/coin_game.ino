/*
  A simple game. Move your player, grab the coin.
*/
#include <Arduino.h>
#include <Bounce.h>

#include "FastLED.h"

#define NUMLEDS 16
#define STRIPPIN 12
#define BRIGHTNESS 48

#define BUTPIN1 0
#define BUTPIN2 1
#define BUTDELAY 20

// buttons
Bounce button1 = Bounce(BUTPIN1, BUTDELAY);
Bounce button2 = Bounce(BUTPIN2, BUTDELAY);

// leds
unsigned int pos = 0;
CRGB leds[NUMLEDS];

// timing
unsigned long timeStep = 100;
unsigned long timer = 0;

// game
bool victory = false;
unsigned int coinX = 0;
unsigned int coinY = 0;
unsigned int playerX = 0;
unsigned int playerY = 0;

void setup() {
  // leds
  FastLED.addLeds<NEOPIXEL, STRIPPIN>(leds, NUMLEDS);

  // buttons
  pinMode(BUTPIN1, INPUT_PULLUP);
  pinMode(BUTPIN2, INPUT_PULLUP);

  // init game state
  srand(analogRead(A0));
  do {
    playerX = rand() % 4;
    playerY = rand() % 4;
    coinX = rand() % 4;
    coinY = rand() % 4;
  } while (coinX == playerX && coinY == playerY);
}

unsigned int mapLED(unsigned int x, unsigned int y) {
  //assert(x < 4);
  //assert(y < 4);
  bool const evenY = !(y % 2);
  return (4 * y) + (evenY ? x : 3 - x);
}

void setAllBlack() {
  for (size_t i = 0; i < NUMLEDS; i++) {
    leds[i] = CHSV(0,0,0);
  }
}

void updateLED() {
  setAllBlack();
  leds[mapLED(playerX, playerY)] = CHSV(0, 255, BRIGHTNESS);
  leds[mapLED(coinX, coinY)] = CHSV(20, 100, BRIGHTNESS);
}

void input() {
  button1.update();
  button2.update();
}

void drawVictoryDance() {
  setAllBlack();

  unsigned int const maxHue = 255;
  for (unsigned int i = 0; i < 10; ++i) {
    for (unsigned int hue = 0; hue <= maxHue; hue += (maxHue / 10)) {
      leds[mapLED(playerX, playerY)] = CHSV(hue, 255, BRIGHTNESS);
      FastLED.show();
      delay(10);      
    }
  }
}

void update() {
  // player movement
  if (button1.fallingEdge()) { playerX = (playerX + 1) % 4; }
  if (button2.fallingEdge()) { playerY = (playerY + 1) % 4; }

  // check win condition
  if (button1.fallingEdge() || button2.fallingEdge()) {
    if (playerX == coinX && playerY == coinY) {
      victory = true;

      // place a new coin
      do {
        coinX = rand() % 4;
        coinY = rand() % 4;
      } while (coinX == playerX && coinY == playerY);
    }
  }

  updateLED();
}

void draw() {
  int pot = analogRead(A1);
  pot = map(pot, 10, 1020, 0, 50);
  timeStep = pot;

  if (millis() - timer > timeStep) {
    if (victory) {
      victory = false;
      drawVictoryDance();
    } else {
      FastLED.show();
    }
    timer = millis();
  }
}

void loop() {
  input();
  update();
  draw();
}