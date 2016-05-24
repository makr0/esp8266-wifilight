bool shouldSaveConfig = false;
bool fs_mounted = false;

void configFS_mount() {
    if(fs_mounted) return;
    if (SPIFFS.begin()) {
        fs_mounted = true;
        Serial.println("mounted file system");
    } else {
        Serial.println("failed to mount FS");
    }
}
void configFS_save() {
    configFS_mount();
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
    shouldSaveConfig = 0;
}

void configFS_handle() {
  if( shouldSaveConfig ) configFS_save();
}