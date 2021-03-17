#include <ArduinoOTA.h>
#include <FastLED.h>
#include <WiFi.h>
#include <Ethernet.h>
#include <heltec.h>

#define NUM_LEDS 300
#define DATA_PIN 2

const char* ssid = "Roll for initiative";
const char* password = "wedembois";

WiFiServer server(12345);

CRGB leds[NUM_LEDS];

void setup()
{
  Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, 470E6 /**/);

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 400);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]()
  {
    Heltec.display->clear();
    Heltec.display->setFont(ArialMT_Plain_10);        //设置字体大小
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);//设置字体对齐方式
    Heltec.display->drawString(0, 0, "Start Updating....");

    Serial.printf("Start Updating....Type:%s\n", (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem");
  });

  ArduinoOTA.onEnd([]()
  {
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Update Complete!");
    Serial.println("Update Complete!");

    ESP.restart();
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    String pro = String(progress / (total / 100)) + "%";
    int progressbar = (progress / (total / 100));
    //int progressbar = (progress / 5) % 100;
    //int pro = progress / (total / 100);

    Heltec.display->clear();
    Heltec.display->drawProgressBar(0, 32, 120, 10, progressbar);    // draw the progress bar
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);          // draw the percentage as String
    Heltec.display->drawString(64, 15, pro);
    Heltec.display->display();

    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("Error[%u]: ", error);
    String info = "Error Info:";
    switch (error)
    {
      case OTA_AUTH_ERROR:
        info += "Auth Failed";
        Serial.println("Auth Failed");
        break;

      case OTA_BEGIN_ERROR:
        info += "Begin Failed";
        Serial.println("Begin Failed");
        break;

      case OTA_CONNECT_ERROR:
        info += "Connect Failed";
        Serial.println("Connect Failed");
        break;

      case OTA_RECEIVE_ERROR:
        info += "Receive Failed";
        Serial.println("Receive Failed");
        break;

      case OTA_END_ERROR:
        info += "End Failed";
        Serial.println("End Failed");
        break;
    }

    Heltec.display->clear();
    Heltec.display->drawString(0, 0, info);
    ESP.restart();
  });
  ArduinoOTA.begin();

  server.begin();
}

void Task1Code(void *parameter) {
  for (;;) {
    FastLED.show();
    delay(8);
  }
}

void loop() {
  ArduinoOTA.handle();

  // Turn the LED on, then pause
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(500);

  WiFiClient client = server.available();

  if (client) {                             // if you get a client,
//    TaskHandle_t Task1;
//    xTaskCreatePinnedToCore(
//      Task1code, /* Function to implement the task */
//      "Task1", /* Name of the task */
//      10000,  /* Stack size in words */
//      NULL,  /* Task input parameter */
//      0,  /* Priority of the task */
//      &Task1,  /* Task handle. */
//      0=
//    );
    while (client.connected()) {            // loop while the client's connected
      int  i = 0;
      while (client.available() > 0) {             // if there's bytes to read from the client,
        if (client.readBytes( (char*)leds, NUM_LEDS * 3) != 0) {
          i++;
          i = i % NUM_LEDS;
          FastLED.show();
        }
      }
    }
    // close the connection:
    client.stop();
  }
}
