void pat_solidColor_start(float h, float s, float l,int speed = 2000) {
  debug_print("color ");
  debug_print(h);debug_print(',');
  debug_print(s);debug_print(',');
  debug_println(l);
  globalState.color[0] = HslColor(h,s,l);
  effect_active = 1;
  animations.StartAnimation(0, speed, [](const AnimationParam& param) {
    float progress = NeoEase::CircularOut(param.progress);

    for (uint16_t i=0; i<=progress * strip.PixelCount(); i++) {
        strip.SetPixelColor(i, globalState.color[0]);
    }
    if (param.state == AnimationState_Completed) effect_active=0;
  });
}

// parameter: h,s,l
void pat_solidColor( JsonObject *out ) {
  int i,speed=2000;
  float h,s,l;

  if (server.args() > 1 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "h") h = server.arg(i).toFloat();
      if (server.argName(i) == "s") s = server.arg(i).toFloat();
      if (server.argName(i) == "l") l = server.arg(i).toFloat();
      if (server.argName(i) == "speed") speed = server.arg(i).toInt();
    }
    globalState.color[0] = HslColor(h,s,l);
  } else {
    h = globalState.color[0].H;
    s = globalState.color[0].S;
    l = globalState.color[0].L;
  }
  pat_solidColor_start(h,s,l,speed);

  out->set("h",h);
  out->set("s",s);
  out->set("l",l);
}

