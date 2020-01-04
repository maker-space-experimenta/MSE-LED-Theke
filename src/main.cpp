// somehow LED loop leads to MQTT timeouts / reconnects ||| TODO: fix
// Art-Net?
// OTA?

#include <Arduino.h>

#include "globals.h"
#include "eth.h"
#include "leds.h"
#include "mqttLogic.h"


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    
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