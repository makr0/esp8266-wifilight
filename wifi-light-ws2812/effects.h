
AnimEaseFunction moveEase =
      NeoEase::Linear;
//      NeoEase::QuadraticInOut;
//      NeoEase::CubicInOut;
//      NeoEase::QuarticInOut;
//      NeoEase::QuinticInOut;
//      NeoEase::SinusoidalInOut;
//      NeoEase::ExponentialInOut;
//      NeoEase::CircularInOut;

#include "patterns/rainbow.h"
#include "patterns/solidcolor.h"
#include "patterns/lauflichter.h"

const static struct {
  const char *name;
  void (*func)(JsonObject*);
} effect_functions [] = {
  { "rainbow_shaded", pat_rainbow },
  { "lauflichter", pat_lauflichter },
  { "one_color", pat_solidColor },
};

void listEffects(JsonArray *data) {
    for (uint8_t i = 0; i < (sizeof(effect_functions) / sizeof(effect_functions[0])); i++) {
        data->add( effect_functions[i].name) ;
    }
}
