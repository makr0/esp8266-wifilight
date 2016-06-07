#define DEBUG
#include "debugging.h"

#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
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


#define pixelCount 16
#define NUM_ANIMATIONS 10
#define LOCAL_HOSTNAME "wifi-light-dev"
#define OLED_address  0x3c
#define SERIAL_SPEED 74880

NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(pixelCount, 4);
NeoPixelAnimator animations(NUM_ANIMATIONS+1);
ESP8266WebServer server ( 80 );
Adafruit_SSD1306 oled_display(0);
byte mac[6]; // MAC address
String macString;
String ipString;
String netmaskString;
String gatewayString;

bool effect_active = false;
bool wifi_setup_done = false;

#include "helpers.h"
#include "configStore.h"
configStore config;

#include "color_conversions.h"
#include "drivers/oled_display.h"
#include "ledstate.h"
#include "effects.h"
#include "protocols/ota.h"
#include "protocols/ssdp.h"
#include "protocols/philips_hue.h"
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
    pat_solidColor( &effect_message );

    WiFi.begin ( config.getValue("ssid"), config.getValue("password") );
    Serial.begin ( SERIAL_SPEED );
    debug_println ( "Application starts" );
}
void loop ( void ) {
  if(!wifi_setup_done ) {
    unsigned long currentMillis = millis();
    if( WiFi.status() == WL_CONNECTED ) {
      macString = String(WiFi.macAddress());
      ipString = StringIPaddress(WiFi.localIP());
      netmaskString = StringIPaddress(WiFi.subnetMask());
      gatewayString = StringIPaddress(WiFi.gatewayIP());
      debug_print ( "IP: " );
      debug_println ( WiFi.localIP() );
      OLED_clear();
      oled_display.println ( config.getValue("ssid") );
      oled_display.print ( "IP: " );
      oled_display.println ( WiFi.localIP() );
      setHTTPHandlers();
      server.begin();
      oled_display.println ( "HTTP server started" );
      SSDPSetup();
      OTASetup();
      wifi_setup_done = true;
      oled_display.display();
      digitalWrite(LED_BUILTIN, HIGH);
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
    config.handle();
  }
  if( effect_active ) {
    animations.UpdateAnimations();
    strip.Show();
  }

}
