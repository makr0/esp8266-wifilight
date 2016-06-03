void hue_addConfigJson(JsonObject *root) {
    debug_println("handle ConfigJson");
    root->set("name", "the flame" );
    root->set("modelid", "flame001");
    root->set("swversion", "0.1" );
    root->set("mac", macString.c_str() );
    root->set("dhcp", true );
    root->set("ipaddress", ipString.c_str() );
    root->set("netmask", netmaskString.c_str() );
    root->set("gateway", gatewayString.c_str() );
    root->set("linkbutton", true ); // Indicates whether the link button has been pressed within the last 30 seconds.
                                    // since we dont have any buttons we indicate that connecting is ok
    root->set("portalservices", false);
    JsonObject& whitelist = root->createNestedObject("whitelist");
    JsonObject& whitelistFirstEntry = whitelist.createNestedObject("dummy#everyone#has#access");
    whitelistFirstEntry["name"]= "dummy user";
    JsonObject& swupdate = root->createNestedObject("swupdate");
    swupdate["text"].set( "" );
    swupdate["notify"].set( false ); // Otherwise client app shows update notice
    swupdate["updatestate"].set( 0 );
    swupdate["url"].set( "" );
}
void hue_addLightJson( JsonObject *root) {
    debug_println("handle LightJson");
    JsonObject& light = root->createNestedObject("1");
    light[ "type" ] = "Extended color light"; // type of lamp (all "Extended colour light" for now)
    light[ "name" ] = "The Cone"; // the name as seen in the web UI or app
    light[ "modelid" ] = "LST001"; // the model number
    light[ "swversion" ] = "65003148";

    JsonObject& state = light.createNestedObject("state");

    if (globalState.color[0].L == 0) {
        state["on"] = false;
    } else {
        state["on"] = true;
    }
    HsbColor hsb = hsl2philips_hsb(globalState.color[0]);
    state[ "hue" ] = hsb.H; // hs mode: the hue (expressed in ~deg*182.04)
    state[ "bri" ] = hsb.B; // brightness between 0-254 (NB 0 is not off!)
    state[ "sat" ] = hsb.S; // hs mode: saturation between 0-254
    JsonArray& xy = state.createNestedArray("xy");
    xy.add(0);xy.add(0);// xy mode: CIE 1931 color co-ordinates
    state[ "ct" ]        =  500; // ct mode: color temp (expressed in mireds range 154-500)
    state[ "alert" ]     =  "none"; // 'select' flash the lamp once, 'lselect' repeat flash for 30s
    state[ "effect" ]    =  "none"; // 'colorloop' makes Hue cycle through colors
    state[ "colormode" ] =  "hs"; // the current color mode
    state[ "reachable" ] =  true; // lamp can be seen by the hub
    JsonObject& pointsymbol = light.createNestedObject("pointsymbol");
    pointsymbol["1"] = "none";
    pointsymbol["2"] = "none";
    pointsymbol["3"] = "none";
    pointsymbol["4"] = "none";
    pointsymbol["5"] = "none";
    pointsymbol["6"] = "none";
    pointsymbol["7"] = "none";
    pointsymbol["8"] = "none";
}

void hue_addApiJson( JsonObject *root) {
    debug_println("handle apijson");
    JsonObject& groups = root->createNestedObject("groups");
    JsonObject& scenes = root->createNestedObject("scenes");
    JsonObject& config = root->createNestedObject("config");
    JsonObject& schedules = root->createNestedObject("schedules");
    JsonObject& lights = root->createNestedObject("lights");

    hue_addConfigJson( &config );
    hue_addLightJson( &lights );
}

// On the real bridge, the link button on the bridge must have been recently pressed for the command to execute successfully.
// We just give a success response. Users are not implemented. Yet.
void hue_addUser( JsonObject *root) {
    JsonObject& msg = root->createNestedObject("success");
    String client;
    String requestedUri = server.uri();
    msg["username"] = "dummy#everyone#has#access";
}

void hue_handleState() {
    debug_println("handle state");
    DynamicJsonBuffer jsonBuffer;
    JsonArray& response = jsonBuffer.createArray();
    DynamicJsonBuffer jsonBuffer_in;
    JsonObject& parsedRoot = jsonBuffer_in.parseObject(server.arg("plain"));
    float h = globalState.color[0].H;
    float s = globalState.color[0].S;
    float l = globalState.color[0].L;
    bool onValue;
    if(parsedRoot.containsKey("on")) {
      onValue = parsedRoot["on"];
      JsonObject& result_o = jsonBuffer.createObject();
      JsonObject& result_ov = result_o.createNestedObject("success");
      result_ov["/lights/1/state/on"] = onValue;
      response.add(result_o);
      debug_print("on");
      debug_println(onValue);
    }
    if( parsedRoot.containsKey("hue") ) {
      h = map_range(parsedRoot["hue"], 0,65535,0,1);
      debug_println(h);
      JsonObject& result_h = jsonBuffer.createObject();
      JsonObject& result_hv = result_h.createNestedObject("success");
      result_hv["/lights/1/state/hue"] = parsedRoot["hue"];
      response.add(result_h);
    }
    if( parsedRoot.containsKey("sat") ) {
      s = map_range(parsedRoot["sat"],0,254,0,1);
      debug_println(s);
      JsonObject& result_s = jsonBuffer.createObject();
      JsonObject& result_sv = result_s.createNestedObject("success");
      result_sv["/lights/1/state/sat"] = parsedRoot["sat"];
      response.add(result_s);
    }
    if( parsedRoot.containsKey("bri") ) {
      l = map_range(parsedRoot["bri"],0,254,0,0.5);
      debug_println(l);
      JsonObject& result_b = jsonBuffer.createObject();
      JsonObject& result_bv = result_b.createNestedObject("success");
      result_bv["/lights/1/state/bri"] = parsedRoot["bri"];
      response.add(result_b);
    }
    if( !onValue ) {
      l = 0;
    }
    pat_solidColor_start( h,s,l,1);
    sendJSON_ResponseArray( &response );
}

bool handleHueProtocol() {
  bool have_match = false, have_sent = false;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  String requestedUri = server.uri();
  debug_println("handleHueProtocol: ");
  debug_println(requestedUri);
  if ( requestedUri.endsWith("/config") ) {
    hue_addConfigJson( &root ); have_match = true;
  }
  if ( requestedUri.endsWith("/lights/1") ) {
    hue_addLightJson( &root ); have_match = true;
  }
  else if (     requestedUri.startsWith("/api")
           &&  (requestedUri.lastIndexOf("/") == 4) ) {
    hue_addApiJson( &root ); have_match = true;
  }
  else if ( requestedUri.endsWith("/api") ) {
    hue_addUser( &root ); have_match = true;
  }
  else if (requestedUri.endsWith("/state"))
  {
    hue_handleState(); have_match = true; have_sent = true;
  }

  if( have_match && !have_sent) {
    sendJSON_Response( &root );
  }
  return have_match;
}