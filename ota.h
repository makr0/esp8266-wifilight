void OTASetup() {
  ArduinoOTA.setHostname(LOCAL_HOSTNAME);
  ArduinoOTA.setPassword(ota_key);
  ArduinoOTA.onStart([]() {
    Serial.println("OTA Update starts");
    effect_active = 0;
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA Update end");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA Update Ready");
}
