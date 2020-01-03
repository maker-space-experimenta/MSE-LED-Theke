#include <MQTT.h>
#include <ArduinoJson.h>

#define DISCOVERY_PREFIX "homeassistant"

MQTTClient mqtt;
String clientStr = "stm32-" + String(macStr);



void sendAutoDiscovery() {
    DynamicJsonDocument payload(500);
    String topic = DISCOVERY_PREFIX "/light/" + clientStr;
    payload["~"] = topic;
    payload["name"] = clientStr;
    payload["unique_id"] = clientStr;
    payload["cmd_t"] = "~/set";
    payload["stat_t"] = "~/state";
    payload["schema"] = "json";
    payload["brightness"] = true;
    payload["rgb"] = true;
    // payload["white_value"] = true; // enable for RGBW

    payload["effect"] = true;
    // JsonArray fx = payload.createNestedArray("effect_list");
    // fx.add("rainbow")

    DEBUG.println("Payload:");
    serializeJsonPretty(payload, DEBUG);

    String buf;
    serializeJson(payload, buf);
    mqtt.publish(String(topic + "/config"), buf);

}

void parseHAssCmd(String &payload) {
    DynamicJsonDocument doc(500);
    DeserializationError err = deserializeJson(doc, payload);
    if(err) {
        DEBUG.println("MQTT JSON payload parsing error");
        DEBUG.println(err.c_str());
        return;
    }

    //ToDo, see how I can handle it neatly
    // JsonVariant b = doc["brightness"];
    if(!doc["brightness"].isNull()) {
        uint8_t brightness = doc["brightness"];
    }

    String state = doc["state"];
    String effect = doc["effect"];
    uint8_t transition = doc["transition"];
    uint8_t white_value = doc["white_value"];


}

void mqttMessageHandler(String &topic, String &payload) {
    DEBUG.println("incoming: " + topic + " - " + payload);
}

void connectMqtt() {
    DEBUG.print("Connecting to broker");
    
    uint32_t initStart = millis();
    while(!mqtt.connect(clientStr.c_str())) {
        DEBUG.print(".");
        delay(250);
        if(millis() - initStart > 10000) {
            DEBUG.println("\nTimeout");
            return;
        }
    }
    DEBUG.println(" done.");

    
}

void initMqtt() {
    mqtt.begin(BROKER_IP, net);
    mqtt.onMessage(mqttMessageHandler);

    connectMqtt();
}

void loopMqtt() {
    mqtt.loop();
}