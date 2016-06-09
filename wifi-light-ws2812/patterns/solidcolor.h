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
    if (param.state == AnimationState_Completed) {
        animations.RestartAnimation(param.index);
    }
  });
}

// parameter: speed
// color at index 0 is used
void pat_solidColor( JsonObject *out ) {
  int i,speed=pixelCount*10;
  float h,s,l;

  if (server.args() > 1 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "speed") speed = server.arg(i).toInt();
    }
  }
  pat_solidColor_start(globalState.color[0].H,
                       globalState.color[0].S,
                       globalState.color[0].L,
                       speed);

  out->set("h",globalState.color[0].H);
  out->set("s",globalState.color[0].S);
  out->set("l",globalState.color[0].L);
}

