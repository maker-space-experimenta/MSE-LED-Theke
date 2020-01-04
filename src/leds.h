#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

typedef struct {
    bool state;
    uint8_t brightness;
    uint32_t color;
    String effect;
} ledState_t;

ledState_t curLedState = {
    INIT_STATE,
    INIT_BRIGHTNESS,
    INIT_COLOR,
    "static"
};


void applyLedState() {
    leds.setBrightness(curLedState.brightness);
    if(curLedState.effect == "static") {
        leds.fill(curLedState.color);
        leds.show();
    }
}

void setLedState(bool state) {
    curLedState.state = state;
}

void setLedBrightness(uint8_t brightness) {
    curLedState.brightness = brightness;
    leds.setBrightness(brightness);
}

void setLedColor(uint32_t color) {
    curLedState.color = color;
}

void setLedEffect(String effect) {
    curLedState.effect = effect;
}

void setLedState(ledState_t state) {
    curLedState.state = state.state;
    curLedState.brightness = state.brightness;
    curLedState.color = state.color;
    curLedState.effect = state.effect;

    applyLedState();
}

ledState_t getLedState() {
    return curLedState;
}

void initLeds() {
    leds.begin();
    applyLedState();
}

uint32_t lastLedUpdate = 0;

void loopLeds() {
    if(millis() - lastLedUpdate > 50) {
        lastLedUpdate = millis();
        // simple rainbow fade animation

        if(!curLedState.state) {
            leds.fill(0);
            leds.show();
        }
        else {
            if(curLedState.effect == "static") {
                leds.fill(curLedState.color);
                leds.show();
            }
            else if(curLedState.effect = "rainbow") {
                uint32_t now = millis();
                for(uint16_t i = 0; i < leds.numPixels(); i++) {
                    uint16_t timeOffset = (65535UL / 5000) * (now % 5000);
                    uint16_t ledOffset = (65535UL * i) / (leds.numPixels());
                    uint16_t hue = (timeOffset + ledOffset) % 65536UL;
                    //cycle time = 5000ms
                    leds.setPixelColor(i, leds.ColorHSV(hue));
                    // if(i == 0) {
                    //     Serial.println(hue);
                    // }
                }
                leds.show();
            }
        }
    }
}