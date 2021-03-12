#include <FastLED.h>
#define NUM_LEDS 300

CRGB leds[NUM_LEDS];

void setup()
{
  FastLED.addLeds<WS2812, 2, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 400);
  Serial.begin(2152000);
  Serial.setTimeout(5);
}

void loop() {
  if (Serial.available()) {
    if (Serial.readBytes( (char*)leds, NUM_LEDS * 3 + 1) != 0) {
      //    for (int i = 0; i < NUM_LEDS; i++) {
      //      Serial.print(leds[i].r);
      //      Serial.print(',');
      //      Serial.print(leds[i].g);
      //      Serial.print(',');
      //      Serial.print(leds[i].b);
      //      Serial.print('\n');
      //    }
      FastLED.show();

    }
  }
}
