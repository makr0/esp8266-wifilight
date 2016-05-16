#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "wifi_credentials.h"
#include "helpers.h"
#define pixelCount 16
#define colorSaturation 255
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(pixelCount, 4);
NeoPixelAnimator animations(10);

ESP8266WebServer server ( 80 );
#include "effects.h"
const int led = 13;
int effect_active = 0;
void handleSetEffect() {
  String effect;
  effect_active = 0;
  digitalWrite ( led, 1 );
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "e") {
        effect = urldecode(server.arg(i));
        for (i = 0; i < (sizeof(effect_functions) / sizeof(effect_functions[0])); i++) {
          if (!strcmp(effect_functions[i].name, effect.c_str()) && effect_functions[i].func) {
            effect_functions[i].func();
            effect_active = 1;
          }
        }
      }
    }
  }
  String out = "";
  if( effect_active ) {
    out += "playing effect:"+effect;
  } else {
    out += "effect:"+effect+" not found\n";
    out += "available effects:";
    out += listEffects();
  }
  server.send ( 200, "text/plain", out );
  digitalWrite ( led, 0 );
}
void handleNotFound() {
    digitalWrite ( led, 1 );
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for ( uint8_t i = 0; i < server.args(); i++ ) {
        message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
    }
    server.send ( 404, "text/plain", message );
    digitalWrite ( led, 0 );
}
void setup ( void ) {
    pinMode ( led, OUTPUT );
    digitalWrite ( led, 0 );
    strip.Begin();
    pat_solidColor();
    strip.Show();

    Serial.begin ( 115200 );
    WiFi.begin ( ssid, password );
    Serial.println ( "" );
    // Wait for connection
    while ( WiFi.status() != WL_CONNECTED ) {
        delay ( 500 );
        Serial.print ( "." );
    }
    Serial.println ( "" );
    Serial.print ( "Connected to " );
    Serial.println ( ssid );
    Serial.print ( "IP address: " );
    Serial.println ( WiFi.localIP() );
    server.on ( "/effect", handleSetEffect );
    server.onNotFound ( handleNotFound );
    server.begin();
    Serial.println ( "HTTP server started" );
}
void loop ( void ) {
    server.handleClient();
    if( effect_active ) {
      animations.UpdateAnimations();
      strip.Show();
    }
}
