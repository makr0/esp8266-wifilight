#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "credentials.h"
#include "helpers.h"

#define LOCAL_HOSTNAME "readFS-dev"
#define OLED_address  0x3c

Adafruit_SSD1306 oled_display(0);

int wifi_setup_done = 0;
#include "oled_display.h"
#include "ota.h"

//flag for saving data
bool shouldSaveConfig = false;
void listFiles() {
  FSInfo fs_info;
  SPIFFS.info(fs_info);

  Dir dir = SPIFFS.openDir("/");
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  JsonObject& fsInfo = json.createNestedObject("info");
  fsInfo["size"] = fs_info.totalBytes;
  fsInfo["used"] = fs_info.usedBytes;
  JsonArray& files = json.createNestedArray("files");

  while (dir.next()) {
    File f = dir.openFile("r");
    JsonObject& fileInfo = jsonBuffer.createObject();
    fileInfo["name"]=dir.fileName();
    fileInfo["size"]=f.size();
    files.add( fileInfo );
  }
  json.printTo(Serial);
}
void testFS() {
  Serial.println("mounting FS...");
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    listFiles();
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(ssid, json["ssid"]);
          strcpy(password, json["password"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    } else {
      Serial.println("config.json not found");
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
}
void setup ( void ) {
    initializeOLED_display();
    WiFi.begin ( ssid, password );
    Serial.begin ( 115200 );
    Serial.println ( "Application starts" );
    testFS();
}

void loop ( void ) {
  if(!wifi_setup_done ) {
    unsigned long currentMillis = millis();
    if( WiFi.status() == WL_CONNECTED ) {
      OLED_clear();
      oled_display.println ( ssid );
      oled_display.print ( "IP: " );
      oled_display.println ( WiFi.localIP() );
      OTASetup();
      wifi_setup_done = 1;
      oled_display.display();
      shouldSaveConfig = 1;
    }
  } else {
    ArduinoOTA.handle();
  }
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["ssid"] = ssid;
    json["password"] = password;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
    shouldSaveConfig = 0;
  }
}
