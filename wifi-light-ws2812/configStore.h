
class configStore {
private:
    bool shouldSaveConfig = false;
    bool fs_mounted = false;
    void do_saveAll() {
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
public:
    configStore(void)
    {
        if (SPIFFS.begin()) {
            fs_mounted = true;
            debug_println(F("mounted file system"));
        } else {
            debug_println(F("failed to mount FS"));
        }
    };

    void handle() {
        if( shouldSaveConfig ) {
            do_saveAll();
            shouldSaveConfig = 0;
        }
    }
    void save() {
        shouldSaveConfig = true;
    }
}