#include <FastLED.h>

char buffer[24];
int len = 0;
CRGB leds[100];

void setup()
{
  FastLED.addLeds<WS2812, 2, GRB>(leds, 100);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0)
  {

    int incomingByte = Serial.read();
    buffer[len++] = incomingByte;
    //
    // check for overflow
    //
    if (len >= 24)
    {
      // overflow, resetting
      len = 0;
    }
    //
    // check for newline (end of message)
    //
    if (incomingByte == '\n')
    {
      int digit, red, green, blue;
      int n = sscanf(buffer, "%d %d %d %d", &digit, &red, &green, &blue);
      if (n == 4)
      {
        // valid message received, use values here..
        leds[digit] =  CRGB(red, green, blue);
        //        for (int i = 0; i < 50; i++)
        //        {
        //          leds[i] = CRGB(red, green, blue);
        //        }
        FastLED.show();
      }
      else
      {
        // parsing error, reject
        //        for (int i = 0; i < 50; i++)
        //        {
        //          leds[i] = CRGB(0, 0, 0);
        //        }
        //        FastLED.show();
      }
      len = 0; // reset buffer counter
    }
  }

}
