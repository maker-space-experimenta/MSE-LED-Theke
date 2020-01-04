#include "config.h"

#define DEBUG Serial1

//1,65m => 99 LEDs
#define NUM_LEDS    140   
#define LED_PIN     PA0
#define INIT_STATE  false
#define INIT_BRIGHTNESS  255
#define INIT_COLOR  0xFFFFFF
// extern CRGBW leds[];
// extern CRGB *ledsRGB;

#define ARTNET_TIMEOUT 5000 // start idle animation after 5s of Artnet missing

#define BROKER_IP "mse-pi.lan"