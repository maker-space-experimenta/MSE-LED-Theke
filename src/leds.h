#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void initLeds() {
    leds.begin();
    leds.setBrightness(INIT_BRIGHTNESS);
    leds.fill(INIT_COLOR);
    leds.show();
}

void loopLeds() {
    // simple rainbow fade animation
    uint32_t now = millis();
    for(uint16_t i = 0; i < leds.numPixels(); i++) {
        uint8_t hue = (255UL * i) / (leds.numPixels() - 1);
        //cycle time = 5000ms
        uint16_t offset = (now % 5000) / (5000 / leds.numPixels());
        leds.setPixelColor((i + offset) % leds.numPixels(), leds.ColorHSV(hue));
    }
    leds.show();
}