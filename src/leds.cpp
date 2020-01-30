#include "leds.h"
#include "ledAnimation.h"
#include <Adafruit_NeoPixel.h>

#include "globals.h"

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_BRG + NEO_KHZ800);

 const char* effectNames[] = {
    "static",
};
int numEffects = sizeof(effectNames) / sizeof(effectNames[0]); 

fx_t effectStringToFx(String effectName) {
    fx_t result = fx_undefined;
    for(int i = 0; i < numEffects; i++) {
        if(effectName == String(effectNames[i])) {
            result = (fx_t)i;
            break;
        }
    }
    return result;
}

ledState_t curLedState = {
    INIT_STATE,
    INIT_BRIGHTNESS,
    INIT_COLOR,
    fx_animation, // initialize to animation 0
    5000
};

// void applyLedState() {
//     leds.setBrightness(curLedState.brightness);
//     if (curLedState.effect == "static") {
//         leds.fill(curLedState.color);
//         leds.show();
//     }
// }

void setLedState(bool state) {
    curLedState.state = state;
}

void setLedBrightness(uint8_t brightness) {
    curLedState.brightness = brightness;
    leds.setBrightness(brightness);
}

void setLedColor(uint32_t color) {
    curLedState.color = color;
    setAnimationBaseColor(color);
}

void setLedAnimationTime(uint32_t transition) {
    curLedState.animationTime = transition;
}

void setLedEffect(String effect) {
    fx_t fx = effectStringToFx(effect);
    if(fx != fx_undefined) {
        curLedState.effect = fx;
    }
    else {
        uint8_t anim = animationNameToId(effect.c_str());
        if(anim > -1) {
            setAnimation(anim);
            curLedState.effect = fx_animation;
        }
    }
} 

const char* getCurLedEffectName() {
    if(curLedState.effect != fx_animation) {
        return effectNames[curLedState.effect];
    }
    else {
        return getCurAnimationName();
    }
} 

void setLedState(ledState_t state) {
    curLedState.state = state.state;
    curLedState.brightness = state.brightness;
    curLedState.color = state.color;
    curLedState.effect = state.effect;
    curLedState.animationTime = state.animationTime;

    // applyLedState();
}

ledState_t getLedState() {
    return curLedState;
}

void setLed(uint16_t i, uint32_t c) {
    leds.setPixelColor(i, c);
}

uint32_t rgb2hsv(uint8_t h, uint8_t s, uint8_t v) {
    return leds.ColorHSV(h * 256, s, v);
}

void initLeds() {
    leds.begin();
    leds.setBrightness(curLedState.brightness);
    initAnimation(NUM_LEDS, setLed, rgb2hsv);
}

uint32_t lastLedUpdate = 0, lastAnimationStep = 0;

void loopLeds() {
    if (millis() - lastLedUpdate > ANIMATION_SPEED) {
        lastLedUpdate = millis();

        if (!curLedState.state) {
            leds.fill(0);
            leds.show();
        }
        else {
            switch(curLedState.effect) {
                case fx_static:
                    leds.fill(curLedState.color);
                    leds.show();
                    break;
                case fx_animation:
                    if(millis() - lastAnimationStep > (curLedState.animationTime / 256)) {
                        // TODO: not sure if I want to skip animation steps, or freeze when timing is not met
                        lastAnimationStep = millis();
                        stepAnimation();
                        leds.show();
                    }
                    break;
                default:
                    break; 
            }
        }
    }
}