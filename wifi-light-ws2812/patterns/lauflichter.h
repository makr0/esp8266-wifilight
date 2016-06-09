
void animFN_lauflichter(const AnimationParam& param) {
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
void pat_lauflichter( JsonObject *out ) {
    int fadeSpeed=1;
    int speed=pixelCount*50;
    int8_t fade=1;

    if (server.args() > 0 ) { for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "speed") speed = server.arg(i).toInt();
      if (server.argName(i) == "fade")  fade = server.arg(i).toInt();
    }}

    for( int i=0; i < NUM_ANIMATIONS/2; i++) {
        animState[i].speed=(i+1)*speed/2+speed;
        animState[i].lastPixel=0;
        animState[i].moveDir=1;
        animations.StartAnimation(i, animState[i].speed, animFN_lauflichter );
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
    out->set("result", String( "starting "+String(NUM_ANIMATIONS/2)+" lights.") );
}
