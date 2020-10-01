/*
 * Alfonso Vila 2020
 * MQTT Client with HomeAssistant discovery structure and objects (more or less)
 */

#include <PubSubClient.h>


WiFiClient espClient;
PubSubClient mqttClient(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void mqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void mqttSetTopics() {
  mqttClient.publish(String("homeassistant/sensor/") + String(MQTT_NAME) + String("/display_text/config"), 
    String("{\"icon\": \"mdi:new-box\",\"unique_id\": \"")
     + String(MQTT_NAME)
     + String("\",\"name\": \"Garaje ESPHome Version\",\"state_topic\": \"garaje/sensor/garaje_esphome_version/state\",\"availability_topic\": \"casa/garaje/status\","\"device\": {\"identifiers\": \"6001942ba582\",\"name\": \"garaje\",\"sw_version\": \"esphome v1.13.6 Oct 12 2019, 21:43:45\",\"model\": \"PLATFORMIO_NODEMCUV2\",\"manufacturer\": \"espressif\" } }"));
  
  mqttClient.subscribe("#");
}

void mqttReconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(":");
    }

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(MQTT_NAME)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic","hello world");
      // ... and resubscribe
      mqttClient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  mqttSetTopics();
}

void setupMqtt() {
#ifdef MQTT_BROKER
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
#endif
}
