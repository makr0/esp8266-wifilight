#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "wifi_credentials.h"
#include "helpers.h"

#define pixelCount 16
#define NUM_ANIMATIONS 5
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(pixelCount, 4);
NeoPixelAnimator animations(NUM_ANIMATIONS+1);

ESP8266WebServer server ( 80 );

int effect_active = 0;
int wifi_setup_done = 0;
#include "ledstate.h"
#include "effects.h"
void handleSetEffect() {
  String effect;
  effect_active = 0;
  for( int i=0; i <= NUM_ANIMATIONS; i++) {
      animations.StopAnimation(i);
  }
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
  }
  server.send ( 200, "text/plain", out );
}
void handleListEffects() {
  String out = "";
  out += "available effects:";
  out += listEffects();
  server.send ( 200, "text/plain", out );
}
void handleNotFound() {
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
}

void setup ( void ) {
    initializeLedstate();
    strip.Begin();
    effect_active=1;
    pat_lauflicht();

    WiFi.begin ( ssid, password );
    Serial.begin ( 115200 );
    Serial.println ( "" );
}
void loop ( void ) {
  if(!wifi_setup_done ) {
    if( WiFi.status() == WL_CONNECTED ) {
      Serial.println ( "" );
      Serial.print ( "Connected to " );
      Serial.println ( ssid );
      Serial.print ( "IP address: " );
      Serial.println ( WiFi.localIP() );
      server.on ( "/effect", handleSetEffect );
      server.on ( "/effects", handleListEffects );
      server.onNotFound ( handleNotFound );
      server.begin();
      Serial.println ( "HTTP server started" );
      wifi_setup_done = 1;
    }
  } else {
    server.handleClient();
  }
  if( effect_active ) {
    animations.UpdateAnimations();
    strip.Show();
  }
}
