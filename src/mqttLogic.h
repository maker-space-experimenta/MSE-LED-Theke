#include <MQTT.h>
#include <ArduinoJson.h>

#define DISCOVERY_PREFIX "homeassistant"

MQTTClient mqtt(512);
String clientStr, baseTopic;


void sendAutoDiscovery() {
    DynamicJsonDocument payload(500);
    
    payload["~"] = baseTopic;
    payload["ret"] = true; //enable retain of messages on MQTT broker
    payload["name"] = clientStr;
    payload["unique_id"] = clientStr;
    payload["cmd_t"] = "~/set";
    payload["stat_t"] = "~/state";
    payload["schema"] = "json";
    payload["brightness"] = true;
    payload["rgb"] = true;

    payload["effect"] = true;
    JsonArray fx = payload.createNestedArray("effect_list");
    fx.add("static");
    fx.add("rainbow");

    // those two options make autodiscovery not work anymore somehow
    // payload["white_value"] = true; // enable for RGBW
    // payload["transition"] = true;

    // DEBUG.println("Payload:");
    // serializeJsonPretty(payload, DEBUG);

    String buf;
    serializeJson(payload, buf);
    mqtt.publish(String(baseTopic + "/config"), buf); 

}


void sendLedState() {
    ledState_t state = getLedState();
    DynamicJsonDocument payload(500);
    payload["state"] = state.state ? "ON" : "OFF";
    payload["brightness"] = state.brightness;
    payload["effect"] = state.effect;
    JsonObject c = payload.createNestedObject("color");
    c["r"] = (state.color >> 16) & 0xFF;
    c["g"] = (state.color >> 8) & 0xFF;
    c["b"] = (state.color >> 0) & 0xFF;

    // DEBUG.println(state.color, 16);
    // DEBUG.println("State:");
    // serializeJsonPretty(payload, DEBUG);

    String buf;
    serializeJson(payload, buf);
    mqtt.publish(String(baseTopic + "/state"), buf); 
}


void parseHAssCmd(String &payload) {
    DynamicJsonDocument doc(500);
    DeserializationError err = deserializeJson(doc, payload);
    if(err) {
        DEBUG.println("MQTT JSON payload parsing error");
        DEBUG.println(err.c_str());
        return;
    }

    setLedState(doc["state"] == "ON");
    
    JsonVariant br = doc["brightness"];
    if(!br.isNull()) {
        setLedBrightness(br);
    }

    JsonVariant fx = doc["effect"];
    if(!fx.isNull()) {
        setLedEffect(fx);
    }

    JsonObject c = doc["color"];
    if(!c.isNull()) {
        uint8_t r = c["r"];
        uint8_t g = c["g"];
        uint8_t b = c["b"];
        uint32_t color = (r << 16) | (g << 8) | b;
        setLedColor(color);
    }

    // doesn't work anyways
    // uint8_t transition = doc["transition"];
    // uint8_t white_value = doc["white_value"];

    sendLedState();
}

void mqttMessageHandler(String &topic, String &payload) {
    DEBUG.println("incoming: " + topic + " - " + payload);
    if(topic == baseTopic + "/set") {
        parseHAssCmd(payload);
    }
}

void connectMqtt() {
    DEBUG.print("Connecting to broker");
    
    uint32_t initStart = millis();
    mqtt.setOptions(5, false, 1000);
    while(!mqtt.connect(clientStr.c_str(), MQTT_USER, MQTT_PASS)) {
        DEBUG.print(".");
        delay(250);
        if(millis() - initStart > 10000) {
            DEBUG.println("\nTimeout");
            return;
        }
    }
    DEBUG.println(" done.");
    mqtt.subscribe(baseTopic + "/set");
}

void initMqtt() {
    mqtt.begin(BROKER_IP, net);
    mqtt.onMessage(mqttMessageHandler);

    clientStr = String("stm32-") + String(macStr);
    baseTopic = DISCOVERY_PREFIX "/light/" + clientStr;

    connectMqtt();
    sendAutoDiscovery();
    sendLedState();
}

void loopMqtt() {
    if(!mqtt.connected()) {
        connectMqtt();
    }
    mqtt.loop();
}