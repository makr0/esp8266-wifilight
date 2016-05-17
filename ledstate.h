struct globalState_t {
 HslColor color[NUM_ANIMATIONS];
} globalState;

struct animState_t {
  int speed;
  uint16_t lastPixel; // track the eye position
  int8_t moveDir; // track the direction of movement
};
animState_t animState[NUM_ANIMATIONS];

void initializeLedstate() {
	for( uint8_t i = 0; i<NUM_ANIMATIONS; i++) {
  		globalState.color[i] = HslColor(1.0f * i/NUM_ANIMATIONS  ,1,0.03);
	}
}
