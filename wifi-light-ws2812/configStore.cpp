#include "debugging.h"
#include "credentials.h"
#include <FS.h>
#include "configStore.h"
#include <ArduinoJson.h>

configStore::configStore() {
    shouldSaveConfig = false;
    fs_mounted = false;
    if (SPIFFS.begin()) {
        fs_mounted = true;
        debug_println(F("mounted file system"));
    } else {
        debug_println(F("failed to mount FS"));
    }
}
void configStore::do_saveAll() {
    return;
//    debug_println(F("saving config"));
//    DynamicJsonBuffer jsonBuffer;
//    JsonObject& json = jsonBuffer.createObject();
//    json["ssid"] = ssid;
//    json["password"] = password;
//
//    File configFile = SPIFFS.open("/config.json", "w");
//    if (!configFile) {
//      debug_println(F("failed to open config file for writing"));
//    }
//
//    json.printTo(configFile);
//    configFile.close();
//    debug_println(F("config saved"));
}

void configStore::handle() {
    if( shouldSaveConfig ) {
        do_saveAll();
        shouldSaveConfig = 0;
    }
}
void configStore::save() {
    shouldSaveConfig = true;
}

char * configStore::getValue(String key) {
    File configFile = SPIFFS.open("/config.json", "r");
    char *configValue;
    if (configFile) {
        debug_println(F("opened config file"));
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        if( json.containsKey(key) ) {
            strcpy(configValue, json[key]);
            return configValue;
        } else {
            if( key == "ssid" ) return default_ssid__;
            if( key == "password" ) return default_password__;
            if( key == "ota_key" ) return ota_key;
        }
    }
}

