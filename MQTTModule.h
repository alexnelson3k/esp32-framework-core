// MQTTModule.h

#ifndef MQTT_MODULE_H
#define MQTT_MODULE_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "BaseModule.h"
#include "ParamManager.h"

class MQTTModule : public BaseModule {
private:
    ParamManager& paramManager;

    WiFiClient wifiClient;
    PubSubClient mqttClient;

    bool connected;
    uint8_t retryCount;

    unsigned long lastReconnectAttempt;
    unsigned long lastPublishAll;
    unsigned long lastPublishNetwork;

    void connectMQTT();
    void disconnectMQTT();

    void publishParam(ParamIndex index);
    void publishByFlag(uint16_t flag);
    void publishByTag(uint32_t tag);

    void subscribeTopics();

    String buildTopic(const String& group, const String& paramName);

    static void mqttCallback(char* topic, byte* payload, unsigned int length);

    unsigned long lastHealthPublish;
    void publishHealthMetrics();
    void publishEvent(const String& category, const String& event, const String& message);

public:
    MQTTModule(ParamManager& pm);

    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;

    bool isConnected();
};

#endif