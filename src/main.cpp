#include <Arduino.h>

#include "globals.h"
#include "eth.h"
#include "mqttLogic.h"
#include "leds.h"

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, LOW);
    // delay(500);
    // digitalWrite(LED_BUILTIN, HIGH);
    // delay(500);
    
    DEBUG.begin(115200);
    DEBUG.println("Hello world");    
    
    initEthernet();
    initMqtt();
    initLeds();
}

void loop() {
    loopMqtt();
    loopLeds();
}