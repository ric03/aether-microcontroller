#pragma once

#include <WiFi.h>

/* 
This example uses FreeRTOS softwaretimers as there is no built-in Ticker library
*/
extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include "AsyncMqttClient.h"

#include "mqtt_config.h"

namespace mqtt
{

    AsyncMqttClient mqttClient;
    TimerHandle_t mqttReconnectTimer;
    TimerHandle_t wifiReconnectTimer;

    void connectToWifi()
    {
        Serial.println("Connecting to Wi-Fi...");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }

    void connectToMqtt()
    {
        Serial.println("Connecting to MQTT...");
        mqttClient.connect();
    }

    void WiFiEvent(WiFiEvent_t event)
    {
        Serial.printf("[WiFi-event] event: %d\n", event);
        switch (event)
        {
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            connectToMqtt();
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("WiFi lost connection");
            xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
            xTimerStart(wifiReconnectTimer, 0);
            break;
        }
    }

    void onMqttConnect(bool sessionPresent)
    {
        Serial.println("Connected to MQTT.");
        Serial.print("Session present: ");
        Serial.println(sessionPresent);

        // TODO is the following necessary?

        // uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
        // Serial.print("Subscribing at QoS 2, packetId: ");
        // Serial.println(packetIdSub);

        // mqttClient.publish("test/lol", 0, true, "test 1");
        // Serial.println("Publishing at QoS 0");

        // uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
        // Serial.print("Publishing at QoS 1, packetId: ");
        // Serial.println(packetIdPub1);

        // uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
        // Serial.print("Publishing at QoS 2, packetId: ");
        // Serial.println(packetIdPub2);
    }

    void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
    {
        Serial.println("Disconnected from MQTT.");

        if (WiFi.isConnected())
        {
            xTimerStart(mqttReconnectTimer, 0);
        }
    }

    void onMqttSubscribe(uint16_t packetId, uint8_t qos)
    {
        Serial.println("Subscribe acknowledged.");
        Serial.print("  packetId: ");
        Serial.println(packetId);
        Serial.print("  qos: ");
        Serial.println(qos);
    }

    void onMqttUnsubscribe(uint16_t packetId)
    {
        Serial.println("Unsubscribe acknowledged.");
        Serial.print("  packetId: ");
        Serial.println(packetId);
    }

    void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
    {
        Serial.println("Publish received.");
        Serial.print("  topic: ");
        Serial.println(topic);
        Serial.print("  qos: ");
        Serial.println(properties.qos);
        Serial.print("  dup: ");
        Serial.println(properties.dup);
        Serial.print("  retain: ");
        Serial.println(properties.retain);
        Serial.print("  len: ");
        Serial.println(len);
        Serial.print("  index: ");
        Serial.println(index);
        Serial.print("  total: ");
        Serial.println(total);
    }

    void onMqttPublish(uint16_t packetId)
    {
        Serial.println("Publish acknowledged.");
        Serial.print("  packetId: ");
        Serial.println(packetId);
    }

    void setup()
    {
        Serial.println("Setting up Wifi and MQTT connection...");

        mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
        wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

        WiFi.onEvent(WiFiEvent);

        mqttClient
            .onConnect(onMqttConnect)
            .onDisconnect(onMqttDisconnect)
            .onSubscribe(onMqttSubscribe)
            .onUnsubscribe(onMqttUnsubscribe)
            .onMessage(onMqttMessage)
            .onPublish(onMqttPublish)
            .setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT)
            .setCredentials(MQTT_USERNAME, MQTT_PASSWORD);

        connectToWifi();
    }

    void publishTemperature(const char *topic, float value)
    {
        auto payload = generateTemperaturePayload(value).c_str();
        mqttClient.publish(topic, 0, true, payload);
    }

    void publishHumidity(const char *topic, float value)
    {
        auto payload = generateHumidityPayload(value).c_str();
        mqttClient.publish(topic, 0, true, payload);
    }

    void publishCO2(const char *topic, int value)
    {
        auto payload = generateCo2Payload(value).c_str();
        mqttClient.publish(topic, 0, true, payload);
    }

    String generateTemperaturePayload(float value)
    {
        return generateJson("temperature", String(value, 1), "˚C");
    }

    String generateHumidityPayload(float value)
    {
        return generateJson("humidity", String(value, 1), "%");
    }

    String generateCo2Payload(uint32_t value)
    {
        return generateJson("co2", String(value), "ppm");
    }

    String generateJson(String measurement, String value, String unit)
    {
        return "{\"measurement\":\"" + measurement + "\",\"value\":" + value + ", \"unit\": " + unit + "}";
    }

} // namespace mqtt
