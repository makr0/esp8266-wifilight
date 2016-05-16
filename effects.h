struct globalState_t {
 HslColor color;
} globalState = { HslColor(0,0,0.1) };

uint16_t lastPixel = 0; // track the eye position
int8_t moveDir = 1; // track the direction of movement

AnimEaseFunction moveEase =
//      NeoEase::Linear;
//      NeoEase::QuadraticInOut;
      NeoEase::CubicInOut;
//      NeoEase::QuarticInOut;
//      NeoEase::QuinticInOut;
//      NeoEase::SinusoidalInOut;
//      NeoEase::ExponentialInOut;
//      NeoEase::CircularInOut;

// parameter: h,s,l
void pat_solidColor() {
  int i;
  float h,s,l;
  if (server.args() > 1 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "h") h = urldecode(server.arg(i)).toFloat();
      if (server.argName(i) == "s") s = urldecode(server.arg(i)).toFloat();
      if (server.argName(i) == "l") l = urldecode(server.arg(i)).toFloat();
    }
	globalState.color = HslColor(h,s,l);
    Serial.println ( "set new Color" );
  } else {
    Serial.println ( "using old Color" );
  }

  for (i=0; i<pixelCount; i++) {
    strip.SetPixelColor(i, globalState.color);
  }
}
// parameter: speed, fade
void pat_lauflicht() {
	int speed=2000;
	int8_t fade=10;
	int fadeSpeed=5;

	if (server.args() > 0 ) { for ( uint8_t i = 0; i < server.args(); i++ ) {
	  if (server.argName(i) == "speed") speed = urldecode(server.arg(i)).toInt();
	  if (server.argName(i) == "fade")   fade = urldecode(server.arg(i)).toInt();
	}}
	 // fade the pixels
	animations.StartAnimation(0, fadeSpeed, [fade](const AnimationParam& param) {
		RgbColor color;

	    if (param.state == AnimationState_Completed) {
		    for (uint16_t indexPixel = 0; indexPixel < strip.PixelCount(); indexPixel++)
		    {
		        color = strip.GetPixelColor(indexPixel);
		        color.Darken(fade);
		        strip.SetPixelColor(indexPixel, color);
		    }
		    animations.RestartAnimation(param.index);
	    }
	});
	// move the "eye"
	animations.StartAnimation(1, speed, [](const AnimationParam& param) {
	    // apply the movement animation curve
	    float progress = moveEase(param.progress);

	    // use the curved progress to calculate the pixel to effect
	    uint16_t nextPixel;
	    if (moveDir > 0) {
	        nextPixel = progress * pixelCount;
	    } else {
	        nextPixel = (1.0f - progress) * pixelCount;
	    }

	    // if progress moves fast enough, we may move more than
	    // one pixel, so we update all between the calculated and
	    // the last
	    if (lastPixel != nextPixel) {
	        for (uint16_t i = lastPixel + moveDir; i != nextPixel; i += moveDir)
	        {
	            strip.SetPixelColor(i, globalState.color);
	        }
	    }
	    strip.SetPixelColor(nextPixel, globalState.color);
	    lastPixel = nextPixel;

	    if (param.state == AnimationState_Completed) {
	        // reverse direction of movement
	        moveDir *= -1;

	        // done, time to restart this position tracking animation/timer
	        animations.RestartAnimation(param.index);
	    }
	} );
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
