#include <FastLED.h>
#include <heltec.h>

#define NUM_LEDS 300
#define DATA_PIN 2

CRGB leds[NUM_LEDS];
int incomingByte = 0;


void setup()
{
  Heltec.begin(false /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, 470E6 /**/);

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 400);
  
  Serial.begin(230400);
  Serial.setTimeout(1000);
}

void loop() {
  // Turn the LED on, then pause
//  for (int i = 0; i < NUM_LEDS; i++) {
//    leds[i] = CRGB::Red;
//  }
//  FastLED.show();
//  delay(500);
//  // Now turn the LED off, then pause
//  for (int i = 0; i < NUM_LEDS; i++) {
//    leds[i] = CRGB::Black;
//  }
//  FastLED.show();
//  delay(500);
//  
  while (Serial.available()) {
    incomingByte = Serial.read();
    if(incomingByte == 254){
      if (Serial.readBytes( (char*)leds, NUM_LEDS * 3) != 0) {
        FastLED.show();
      }
    }
  }
}
