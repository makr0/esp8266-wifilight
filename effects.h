
AnimEaseFunction moveEase =
//      NeoEase::Linear;
//      NeoEase::QuadraticInOut;
//      NeoEase::CubicInOut;
//      NeoEase::QuarticInOut;
//      NeoEase::QuinticInOut;
      NeoEase::SinusoidalOut;
//      NeoEase::SinusoidalInOut;
//      NeoEase::ExponentialInOut;
//      NeoEase::CircularInOut;

// parameter: h,s,l
void pat_solidColor() {
  int i,speed=500;
  float h,s,l;
  if (server.args() > 1 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "h") h = urldecode(server.arg(i)).toFloat();
      if (server.argName(i) == "s") s = urldecode(server.arg(i)).toFloat();
      if (server.argName(i) == "l") l = urldecode(server.arg(i)).toFloat();
      if (server.argName(i) == "speed") speed = urldecode(server.arg(i)).toInt();
    }
    globalState.color[0] = HslColor(h,s,l);
    Serial.println ( "set new Color" );
  } else {
    Serial.println ( "using old Color" );
  }

  animations.StartAnimation(0, speed, [](const AnimationParam& param) {
    float progress = NeoEase::CircularOut(param.progress);

    for (uint16_t i=0; i<=progress * strip.PixelCount(); i++) {
        strip.SetPixelColor(i, globalState.color[0]);
    }
    if (param.state == AnimationState_Completed) effect_active=0;
  });
}

void animFN_lauflicht(const AnimationParam& param) {
    float progress = moveEase(param.progress);
    uint16_t nextPixel;
    animState_t state = animState[param.index];

    if (state.moveDir > 0) { nextPixel = progress * strip.PixelCount(); }
    else                   { nextPixel = (1.0f - progress) * strip.PixelCount(); }
    if (state.lastPixel != nextPixel) {
        for (uint16_t i = state.lastPixel + state.moveDir; i != nextPixel; i += state.moveDir) {
            strip.SetPixelColor(i, RgbColor::LinearBlend(
                                       strip.GetPixelColor(i),
                                       globalState.color[param.index],
                                       0.5f)
            );
        }
    }
    strip.SetPixelColor(nextPixel,
        RgbColor::LinearBlend(
            strip.GetPixelColor(nextPixel),
            globalState.color[param.index],
            0.5f)
    );
    state.lastPixel = nextPixel;

    if (param.state == AnimationState_Completed) {
        state.moveDir *= -1;
        animations.RestartAnimation(param.index);
    }
    animState[param.index] = state;
}

// parameter: speed, fade
void pat_lauflicht() {
    int fadeSpeed=5;
    int speed=800;
    int8_t fade=1;

    if (server.args() > 0 ) { for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "speed") speed = urldecode(server.arg(i)).toInt();
      if (server.argName(i) == "fade")  fade = urldecode(server.arg(i)).toInt();
    }}

    Serial.println ( "starting "+String(NUM_ANIMATIONS)+" lights." );
    for( int i=0; i < NUM_ANIMATIONS; i++) {
        animState[i].speed=i*speed/2+speed;
        animState[i].lastPixel=0;
        animState[i].moveDir=1;
        animations.StartAnimation(i, animState[i].speed, animFN_lauflicht );
    }
    animState[NUM_ANIMATIONS].speed=fade;

     // fade the pixels
    animations.StartAnimation(NUM_ANIMATIONS, fadeSpeed, [](const AnimationParam& param) {
        RgbColor color;

        if (param.state == AnimationState_Completed) {
            for (uint16_t indexPixel = 0; indexPixel < strip.PixelCount(); indexPixel++)
            {
                color = strip.GetPixelColor(indexPixel);
                color.Darken(animState[NUM_ANIMATIONS].speed);
                strip.SetPixelColor(indexPixel, color);
            }
            animations.RestartAnimation(param.index);
        }
    });

}

const static struct {
  const char *name;
  void (*func)(void);
} effect_functions [] = {
  { "lauflicht", pat_lauflicht },
  { "solid", pat_solidColor },
};

String listEffects(void) {
    String elist;
    for (uint8_t i = 0; i < (sizeof(effect_functions) / sizeof(effect_functions[0])); i++) {
        elist += effect_functions[i].name;
        elist += ',';
    }
    return elist;
}
