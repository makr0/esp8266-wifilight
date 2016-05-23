
AnimEaseFunction moveEase =
      NeoEase::Linear;
//      NeoEase::QuadraticInOut;
//      NeoEase::CubicInOut;
//      NeoEase::QuarticInOut;
//      NeoEase::QuinticInOut;
//      NeoEase::SinusoidalInOut;
//      NeoEase::ExponentialInOut;
//      NeoEase::CircularInOut;

#include "pat_rainbow.h"

// parameter: h,s,l
void pat_solidColor( JsonObject *out ) {
  int i,speed=2000;
  float h,s,l;

  if (server.args() > 1 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "h") h = urldecode(server.arg(i)).toFloat();
      if (server.argName(i) == "s") s = urldecode(server.arg(i)).toFloat();
      if (server.argName(i) == "l") l = urldecode(server.arg(i)).toFloat();
      if (server.argName(i) == "speed") speed = urldecode(server.arg(i)).toInt();
    }
    globalState.color[0] = HslColor(h,s,l);
  }

  animations.StartAnimation(0, speed, [](const AnimationParam& param) {
    float progress = NeoEase::CircularOut(param.progress);

    for (uint16_t i=0; i<=progress * strip.PixelCount(); i++) {
        strip.SetPixelColor(i, globalState.color[0]);
    }
    if (param.state == AnimationState_Completed) effect_active=0;
  });

  out->set("h",h);
  out->set("s",s);
  out->set("l",l);
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
                                       0.1f)
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
void pat_lauflicht( JsonObject *out ) {
    int fadeSpeed=5;
    int speed=5000;
    int8_t fade=1;

    if (server.args() > 0 ) { for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "speed") speed = urldecode(server.arg(i)).toInt();
      if (server.argName(i) == "fade")  fade = urldecode(server.arg(i)).toInt();
    }}

    for( int i=0; i < NUM_ANIMATIONS/2; i++) {
        animState[i].speed=(i+1)*speed/2+speed;
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
    out->set("result", String( "starting "+String(NUM_ANIMATIONS/2)+" lights.") );
}

const static struct {
  const char *name;
  void (*func)(JsonObject*);
} effect_functions [] = {
  { "rainbow_shaded", pat_rainbow },
  { "lauflichter", pat_lauflicht },
  { "one_color", pat_solidColor },
};

void listEffects(JsonArray *data) {
    for (uint8_t i = 0; i < (sizeof(effect_functions) / sizeof(effect_functions[0])); i++) {
        data->add( effect_functions[i].name) ;
    }
}
