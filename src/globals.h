#include "config.h"
#include <Arduino.h>
#define DEBUG Serial1

//13m, 30LEDs/m, 3 LEDs per IC => 130 LEDs
#define NUM_LEDS    130  
#define LED_PIN     PB4
#define INIT_STATE  false
#define INIT_BRIGHTNESS  255
#define INIT_COLOR  0xFFFFFF

#define ANIMATION_SPEED 20 //speed of animation in ms, 20ms = 50Hz
// extern CRGBW leds[];
// extern CRGB *ledsRGB;

// #define ARTNET_TIMEOUT 5000 // start idle animation after 5s of Artnet missing

#define BROKER_IP "mse-pi.lan"

#define PROGMEM // discard PROGMEM directive on STM32