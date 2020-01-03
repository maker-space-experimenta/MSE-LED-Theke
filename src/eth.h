#include <SPI.h>
#include <Ethernet.h>

uint8_t mac[6];
char macStr[13];
EthernetClient net;

void generateMAC(uint8_t* macArray) {
    
  uint32_t uid0 = HAL_GetUIDw0();
  uint32_t uid1 = HAL_GetUIDw1();
  macArray[0] = 0x42;
  macArray[1] = (uid0 >> 0) & 0xFF;
  macArray[2] = (uid0 >> 8) & 0xFF;
  macArray[3] = (uid0 >> 16) & 0xFF;
  macArray[4] = (uid0 >> 24) & 0xFF;
  macArray[5] = (uid1 >> 0) & 0xFF;

  snprintf(macStr, 13, "%02X%02X%02X%02X%02X%02X", macArray[0], macArray[1], macArray[2], macArray[3], macArray[4], macArray[5]);
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