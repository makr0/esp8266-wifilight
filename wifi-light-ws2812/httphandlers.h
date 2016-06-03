void handleSetEffect() {
  String effect;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& effect_message = jsonBuffer.createObject();
  effect_active = 0;
  for( int i=0; i <= NUM_ANIMATIONS; i++) {
      animations.StopAnimation(i);
  }
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "e") {
        effect = server.arg(i);
        for (i = 0; i < (sizeof(effect_functions) / sizeof(effect_functions[0])); i++) {
          if (!strcmp(effect_functions[i].name, effect.c_str()) && effect_functions[i].func) {
            effect_functions[i].func( &effect_message );
            effect_active = 1;
          }
        }
      }
    }
  }
  JsonObject& root = jsonBuffer.createObject();
  if( effect_active ) {
    root["message"] = effect_message;
  } else {
    root["error"] = String( "effect "+effect+" not found" );
  }
  sendJSON_Response( &root );
}
void handleListEffects() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& data = root.createNestedArray("effects");
  listEffects( &data );
  sendJSON_Response( &root);
}
void handleNotFound() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["error"] = "File Not Found";
  root["uri"]   = server.uri();
  root["method"]= ( server.method() == HTTP_GET ) ? "GET":"POST";
  JsonObject& arguments = root.createNestedObject("arguments");
  for ( uint8_t i = 0; i < server.args(); i++ ) {
      arguments.set( server.argName ( i ) ,server.arg ( i ) );
  }
  root.printTo(Serial);
  sendJSON_Response(&root, 404);
}
void handleStatus() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["freeSketchSpace"]=ESP.getFreeSketchSpace();
  JsonArray& colors = root.createNestedArray("colors");
  for( int i = 0; i < NUM_ANIMATIONS; i++) {
    JsonObject& color = jsonBuffer.createObject();
    color["h"].set( globalState.color[i].H,4 );
    color["s"].set( globalState.color[i].S,4 );
    color["l"].set( globalState.color[i].L,4 );
    colors.add( color );
  }
  sendJSON_Response(&root);
}
void handleListFiles() {
  DynamicJsonBuffer jsonBuffer;
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  Dir dir = SPIFFS.openDir("/");
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
  sendJSON_Response(&json);
}
bool handleFileRead( String path) {
  if (SPIFFS.exists( path )) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, "text/plain");
    return true;
  } else {
    return false;
  }
}

void setHTTPHandlers() {
  server.on ( "/effect", handleSetEffect );
  server.on ( "/effects", handleListEffects );
  server.on ( "/status", handleStatus );
  server.on ( "/files", handleListFiles );
  server.on ( "/description.xml", HTTP_GET, [](){
    debug_println("sending description.xml");
    SSDP.schema(server.client());
  });
  // everything else is interpreted as filename
  server.onNotFound([](){
    if( !handleHueProtocol() ) {
      if(!handleFileRead(server.uri())) {
        handleNotFound();
      }
    }
  });
}
