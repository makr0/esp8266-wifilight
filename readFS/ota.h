void OTASetup() {
  ArduinoOTA.setHostname(LOCAL_HOSTNAME);
  ArduinoOTA.setPassword(ota_key);
  ArduinoOTA.onStart([]() {
    OLED_clear();
    oled_display.println("OTA Update");
    oled_display.drawRect(0,SSD1306_LCDHEIGHT/2,SSD1306_LCDWIDTH-1,5,WHITE);

    oled_display.display();
  });
  ArduinoOTA.onEnd([]() {
    oled_display.println("OTA Update done");oled_display.display();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    int percent = (progress / (total / (SSD1306_LCDWIDTH-2)) );
    if( percent % 10 == 0) {
      oled_display.fillRect(1,SSD1306_LCDHEIGHT/2+1,percent,3,WHITE);
      oled_display.display();
    }
  });
  ArduinoOTA.onError([](ota_error_t error) {
    OLED_clear();
    oled_display.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) oled_display.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) oled_display.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) oled_display.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) oled_display.println("Receive Failed");
    else if (error == OTA_END_ERROR) oled_display.println("End Failed");
    oled_display.display();
  });
  ArduinoOTA.begin();
  oled_display.println("OTA Update Ready");oled_display.display();
}
