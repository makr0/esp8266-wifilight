void sendJSON_Response( JsonObject *root, int status=200 ) {
  String message;
  root->printTo( message );
  server.send ( status, "application/json", message );
}
void handleSetEffect() {
  String effect;
  JsonObject& effect_message = jsonBuffer.createObject();
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
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& data = root.createNestedArray("effects");
  listEffects( &data );
  sendJSON_Response( &root);
}
void handleNotFound() {
  JsonObject& root = jsonBuffer.createObject();
  root["error"] = "File Not Found";
  root["uri"]   = server.uri();
  root["method"]= ( server.method() == HTTP_GET ) ? "GET":"POST";
  JsonObject& arguments = root.createNestedObject("arguments");
  for ( uint8_t i = 0; i < server.args(); i++ ) {
      arguments.set( server.argName ( i ) ,server.arg ( i ) );
  }
  sendJSON_Response(&root, 404);
}
void handleStatus() {
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
void setHTTPHandlers() {
  server.on ( "/effect", handleSetEffect );
  server.on ( "/effects", handleListEffects );
  server.on ( "/status", handleStatus );
  server.onNotFound ( handleNotFound );
}
