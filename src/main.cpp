#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>
// #include <FastLED.h>

#define DEBUG Serial1

// SPIClass SPITwo(PB15, PB14, PB13);

uint8_t mac[6];

void generateMAC(uint8_t* macArray) {
  uint32_t uid0 = HAL_GetUIDw0();
  uint32_t uid1 = HAL_GetUIDw1();
  macArray[0] = 0x42;
  macArray[1] = (uid0 >> 0) & 0xFF;
  macArray[2] = (uid0 >> 8) & 0xFF;
  macArray[3] = (uid0 >> 16) & 0xFF;
  macArray[4] = (uid0 >> 24) & 0xFF;
  macArray[5] = (uid1 >> 0) & 0xFF;
}

void initEthernet() {
    SPI.setMOSI(PB15);
    SPI.setMISO(PB14);
    SPI.setSCLK(PB13);

    Ethernet.init(PB12);

    generateMAC(mac);

    DEBUG.println("Initialize Ethernet with DHCP:");
    if (Ethernet.begin(mac) == 0) {
        DEBUG.println("Failed to configure Ethernet using DHCP");
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
            DEBUG.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
            delay(5000);
            HAL_NVIC_SystemReset();
        } else if (Ethernet.linkStatus() == LinkOFF) {
            DEBUG.println("Ethernet cable is not connected.");
        }
        // no point in carrying on, so do nothing forevermore:
        while (true) {
            delay(1);
        }
    }
    // print your local IP address:
    DEBUG.print("My IP address: ");
    DEBUG.println(Ethernet.localIP());
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, LOW);
    // delay(500);
    // digitalWrite(LED_BUILTIN, HIGH);
    // delay(500);
    
    DEBUG.begin(115200);
    DEBUG.println("Hello world");
    
    
    initEthernet();
}

void loop() {
    // put your main code here, to run repeatedly:
}