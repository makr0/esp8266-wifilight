
void animFN_rainbow(const AnimationParam& param) {
    float progress = moveEase(param.progress);
    animState_t state = animState[param.index];
    float hdelta = 1.0f/strip.PixelCount();
    float h = progress;
    float l = 0.5;
    float pixelpos_rel = 0;

    for (float i=0; i<=strip.PixelCount(); i++) {
      h += hdelta;
      pixelpos_rel=(i / strip.PixelCount());
      l = ( 1 + sin( pixelpos_rel*animState[0].moveDir*2*PI - (progress*2*PI) ) ) / 4.0f * 0.95f;
      l = l + 0.025;
      l = l * 2;
      l = 0.5-constrain(l,0,0.5);

      if( h > 1) { h -= 1; }
      strip.SetPixelColor(i, HslColor( h, globalState.color[1].S, l ) );
    }

    if (param.state == AnimationState_Completed) {
        animations.RestartAnimation(param.index);
    }
}

// parameter: speed, fade
void pat_rainbow( JsonObject *out ) {
    int fadeSpeed=5;
    int speed=5000;
    int loecher = 4;
    int8_t fade=1;

   if (server.args() > 1 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "speed") speed = urldecode(server.arg(i)).toInt();
      if (server.argName(i) == "loecher") loecher = urldecode(server.arg(i)).toInt();
    }
  }
  animState[0].moveDir = loecher;
  animations.StartAnimation(0, speed, animFN_rainbow );

  out->set("result", String( "starting rainbow animation.") );
}
