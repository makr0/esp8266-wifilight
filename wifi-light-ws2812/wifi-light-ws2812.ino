#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "credentials.h"
#include "helpers.h"

#define pixelCount 16
#define NUM_ANIMATIONS 10
#define LOCAL_HOSTNAME "wifi-light-dev"
#define OLED_address  0x3c

NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(pixelCount, 4);
NeoPixelAnimator animations(NUM_ANIMATIONS+1);
ESP8266WebServer server ( 80 );
Adafruit_SSD1306 oled_display(0);

bool effect_active = false;
bool wifi_setup_done = false;

#include "configFS.h"
#include "oled_display.h"
#include "ota.h"
#include "ledstate.h"
#include "effects.h"
#include "httphandlers.h"

const long statusled_blinkinterval_wifi = 75;
int statusledState = HIGH; // means 'OFF'
unsigned long previousMillis = 0;
void setup ( void ) {
    pinMode(LED_BUILTIN, OUTPUT);
    DynamicJsonBuffer jsonBuffer;
    JsonObject& effect_message = jsonBuffer.createObject();
    initializeOLED_display();
    initializeLedstate();
    strip.Begin();
    effect_active=true;
    pat_lauflicht( &effect_message );

    WiFi.begin ( ssid, password );
    Serial.begin ( 115200 );
    Serial.println ( "Application starts" );
}
void loop ( void ) {
  if(!wifi_setup_done ) {
    unsigned long currentMillis = millis();
    if( WiFi.status() == WL_CONNECTED ) {
      OLED_clear();
      oled_display.println ( ssid );
      oled_display.print ( "IP: " );
      oled_display.println ( WiFi.localIP() );
      setHTTPHandlers();
      server.begin();
      oled_display.println ( "HTTP server started" );
      OTASetup();
      wifi_setup_done = true;
      oled_display.display();
      digitalWrite(LED_BUILTIN, HIGH);
      shouldSaveConfig = true;
    } else {
      if(currentMillis - previousMillis >= statusled_blinkinterval_wifi) {
        previousMillis = currentMillis;
        statusledState = statusledState == LOW ? HIGH : LOW;
        digitalWrite(LED_BUILTIN, statusledState);
      }
    }
  } else {
    server.handleClient();
    ArduinoOTA.handle();
    configFS_handle();
  }
  if( effect_active ) {
    animations.UpdateAnimations();
    strip.Show();
  }

}
