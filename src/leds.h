#pragma once
#include <Arduino.h>

typedef struct {
    bool state;
    uint8_t brightness;
    uint32_t color;
    String effect;
} ledState_t;

void initLeds();
void loopLeds();
void setLedState(bool state);
void setLedBrightness(uint8_t brightness);
void setLedColor(uint32_t color);
void setLedEffect(String effect);
ledState_t getLedState();