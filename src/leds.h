#pragma once
#include <Arduino.h>

typedef enum effect {
    fx_undefined        = -1,
    fx_static           = 0,
    fx_animation        = 100,  // internal only
} fx_t;

// Transistion time is being used for animation speed. Defines the speed in seconds for one animation loop (typically 256 steps)
typedef struct {
    bool state;
    uint8_t brightness;
    uint32_t color;
    fx_t effect;
    uint32_t animationTime;
} ledState_t;

extern const char* effectNames[];
extern int numEffects;

void initLeds();
void loopLeds();
void setLedState(ledState_t state);
void setLedState(bool state);
void setLedBrightness(uint8_t brightness);
void setLedColor(uint32_t color);
void setLedAnimationTime(uint32_t transition); // ms
void setLedEffect(String effect);
const char* getCurLedEffectName();
ledState_t getLedState();