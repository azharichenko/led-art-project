#include <FastLED.h>
#define NUM_LEDS 300
#define FPS 24

CRGB leds[NUM_LEDS];

void setup()
{
  FastLED.addLeds<WS2812, 2, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 400);
  Serial.begin(2152000);
  Serial.setTimeout(8);
}

void loop() {
  if (Serial.available()) {
    if (Serial.readBytes( (char*)leds, NUM_LEDS * 3) != 0) {
      FastLED.show();
    }
  }
}
