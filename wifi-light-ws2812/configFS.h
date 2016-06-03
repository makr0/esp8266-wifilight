bool shouldSaveConfig = false;
bool fs_mounted = false;

void configFS_mount() {
    if(fs_mounted) return;
    if (SPIFFS.begin()) {
        fs_mounted = true;
        debug_println(F("mounted file system"));
    } else {
        debug_println(F("failed to mount FS"));
    }
}

// grab some values from gobal variables
// and stuff them into config.json. somehow.
void configFS_save() {
    configFS_mount();
    debug_println(F("saving config"));
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["ssid"] = ssid;
    json["password"] = password;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      debug_println(F("failed to open config file for writing"));
    }

    json.printTo(configFile);
    configFile.close();
    debug_println(F("config saved"));
}

void configFS_handle() {
  if( shouldSaveConfig ) {
    configFS_save();
    shouldSaveConfig = 0;
  }
}