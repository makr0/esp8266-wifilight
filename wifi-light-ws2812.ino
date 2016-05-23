#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <ArduinoJson.h>
#include "credentials.h"
#include "helpers.h"

#define pixelCount 150
#define NUM_ANIMATIONS 10
#define LOCAL_HOSTNAME "wifi-light-dev"
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(pixelCount, 4);
NeoPixelAnimator animations(NUM_ANIMATIONS+1);
ESP8266WebServer server ( 80 );
StaticJsonBuffer<10000> jsonBuffer;

int effect_active = 0;
int wifi_setup_done = 0;
#include "ota.h"
#include "ledstate.h"
#include "effects.h"
#include "httphandlers.h"

void setup ( void ) {
    JsonObject& effect_message = jsonBuffer.createObject();
    initializeLedstate();
    strip.Begin();
    effect_active=1;
    pat_rainbow( &effect_message );

    WiFi.begin ( wifi_ssid, wifi_password );
    Serial.begin ( 115200 );
    Serial.println ( "" );
    Serial.println ( "Application starts" );
}
void loop ( void ) {
  if(!wifi_setup_done ) {
    if( WiFi.status() == WL_CONNECTED ) {
      Serial.println ( "" );
      Serial.print ( "Connected to " );
      Serial.println ( wifi_ssid );
      Serial.print ( "IP address: " );
      Serial.println ( WiFi.localIP() );
      setHTTPHandlers();
      server.begin();
      Serial.println ( "HTTP server started" );
      OTASetup();
      wifi_setup_done = 1;
    }
  } else {
    server.handleClient();
    ArduinoOTA.handle();
  }
  if( effect_active ) {
    animations.UpdateAnimations();
    strip.Show();
  }
}
