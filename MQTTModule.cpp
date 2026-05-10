// MQTTModule.cpp

#include "MQTTModule.h"
#include "Param_Types.h"

extern ConfigParam system_params[PARAM_COUNT];

MQTTModule* globalMQTTModule = nullptr;

// ============================================================
// Constructor
// ============================================================

MQTTModule::MQTTModule(ParamManager& pm)
    : BaseModule("MQTT"),
      paramManager(pm),
      mqttClient(wifiClient),
      connected(false),
      retryCount(0),
      lastReconnectAttempt(0),
      lastPublishAll(0),
      lastPublishNetwork(0),
      lastHealthPublish(0) {

    globalMQTTModule = this;
}

// ============================================================
// Init
// ============================================================

bool MQTTModule::init() {
    if (!paramManager.getBool(PARAM_MQTT_ENABLED)) {
        initialized = false;
        return false;
    }

    mqttClient.setServer(
        paramManager.getString(PARAM_MQTT_SERVER),
        paramManager.getUInt(PARAM_MQTT_PORT)
    );

    mqttClient.setCallback(mqttCallback);

    initialized = true;
    enabled = true;

    return true;
}

// ============================================================
// Start / Stop
// ============================================================

void MQTTModule::start() {
    enabled = true;
}

void MQTTModule::stop() {
    disconnectMQTT();
    enabled = false;
}

bool MQTTModule::isConnected() {
    return mqttClient.connected();
}

// ============================================================
// Main Loop
// ============================================================

void MQTTModule::loop() {
    if (!enabled || !paramManager.getBool(PARAM_MQTT_ENABLED)) {
        if (mqttClient.connected()) {
            disconnectMQTT();
        }
        return;
    }

    if (!mqttClient.connected()) {
        unsigned long now = millis();

        if (now - lastReconnectAttempt >= paramManager.getUInt(PARAM_MQTT_RECONNECT_INTERVAL_MS)) {
            lastReconnectAttempt = now;

            if (retryCount < paramManager.getUInt(PARAM_MQTT_MAX_RETRIES)) {
                connectMQTT();
                retryCount++;
            }
        }

        return;
    }

    retryCount = 0;

    mqttClient.loop();

    unsigned long now = millis();

    // Publish all MQTT-enabled params
    if (now - lastPublishAll >= paramManager.getUInt(PARAM_MQTT_PUBLISH_INTERVAL_MS)) {
        lastPublishAll = now;
        publishByFlag(FLAG_MQTT_ACCESS);
    }

    // Publish network group
    if (now - lastPublishNetwork >= paramManager.getUInt(PARAM_MQTT_PUBLISH_NETWORK_INTERVAL_MS)) {
        lastPublishNetwork = now;
        publishByTag(TAG_NETWORK);
    }

    // Publish health metrics
    if (now - lastHealthPublish >= paramManager.getUInt(PARAM_HEALTH_REPORT_INTERVAL_MS)) {
        lastHealthPublish = now;
        publishHealthMetrics();
    }
}

// ============================================================
// Connect / Disconnect
// ============================================================

void MQTTModule::connectMQTT() {
    String clientId = paramManager.getString(PARAM_DEVICE_NAME);

    bool success = mqttClient.connect(
        clientId.c_str(),
        paramManager.getString(PARAM_MQTT_USERNAME),
        paramManager.getString(PARAM_MQTT_PASSWORD)
    );

    if (success) {
        connected = true;
        subscribeTopics();
    } else {
        connected = false;
    }
}

void MQTTModule::disconnectMQTT() {
    mqttClient.disconnect();
    connected = false;
}

// ============================================================
// Topic Builder
// ============================================================

String MQTTModule::buildTopic(const String& group, const String& paramName) {
    String base = "framework";
    String device = paramManager.getString(PARAM_DEVICE_NAME);

    return base + "/" + device + "/" + group + "/" + paramName;
}

// ============================================================
// Publish Helpers
// ============================================================

void MQTTModule::publishParam(ParamIndex index) {
    ConfigParam& param = system_params[index];

    String topic = buildTopic("status", param.name);
    String payload = getParamDisplayValue(index);

    mqttClient.publish(topic.c_str(), payload.c_str(), true);
}

void MQTTModule::publishByFlag(uint16_t flag) {
    for (uint16_t i = 0; i < PARAM_COUNT; i++) {
        if (system_params[i].flags & flag) {
            publishParam((ParamIndex)i);
        }
    }
}

void MQTTModule::publishByTag(uint32_t tag) {
    for (uint16_t i = 0; i < PARAM_COUNT; i++) {
        if (system_params[i].tags & tag) {
            publishParam((ParamIndex)i);
        }
    }
}

void MQTTModule::publishHealthMetrics() {
    String base = String("framework/") +
                  paramManager.getString(PARAM_DEVICE_NAME) +
                  "/health/";

    mqttClient.publish(
        (base + "uptime").c_str(),
        String(millis() / 1000).c_str(),
        true
    );

    mqttClient.publish(
        (base + "free_heap").c_str(),
        String(ESP.getFreeHeap()).c_str(),
        true
    );

    mqttClient.publish(
        (base + "min_free_heap").c_str(),
        String(ESP.getMinFreeHeap()).c_str(),
        true
    );

    mqttClient.publish(
        (base + "wifi_rssi").c_str(),
        String(WiFi.RSSI()).c_str(),
        true
    );

    mqttClient.publish(
        (base + "ip").c_str(),
        WiFi.localIP().toString().c_str(),
        true
    );

    mqttClient.publish(
        (base + "reset_reason").c_str(),
        String((int)esp_reset_reason()).c_str(),
        true
    );
}

void MQTTModule::publishEvent(
    const String& category,
    const String& event,
    const String& message
) {
    if (!mqttClient.connected()) return;

    String topic = String("framework/") +
               paramManager.getString(PARAM_DEVICE_NAME) +
               "/events/" +
               category;

    String payload = "{";
    payload += "\"event\":\"" + event + "\",";
    payload += "\"timestamp\":" + String(millis()) + ",";
    payload += "\"message\":\"" + message + "\"";
    payload += "}";

    mqttClient.publish(topic.c_str(), payload.c_str(), false);
}
// ============================================================
// Subscribe
// ============================================================

void MQTTModule::subscribeTopics() {
    for (uint16_t i = 0; i < PARAM_COUNT; i++) {
        if (system_params[i].flags & FLAG_MQTT_ACCESS) {
            String topic = buildTopic("set", system_params[i].name);
            mqttClient.subscribe(topic.c_str());
        }
    }
}

// ============================================================
// Callback
// ============================================================

void MQTTModule::mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (!globalMQTTModule) return;

    String incomingTopic = String(topic);

    String value;
    for (unsigned int i = 0; i < length; i++) {
        value += (char)payload[i];
    }

    for (uint16_t i = 0; i < PARAM_COUNT; i++) {
        String expectedTopic = globalMQTTModule->buildTopic(
            "set",
            system_params[i].name
        );

        if (incomingTopic == expectedTopic) {

            switch (system_params[i].type) {

                case TYPE_BOOL:
                    globalMQTTModule->paramManager.setBool(
                        (ParamIndex)i,
                        value == "1" || value == "true" || value == "on"
                    );
                    break;

                case TYPE_UINT8:
                case TYPE_UINT16:
                case TYPE_UINT32:
                    globalMQTTModule->paramManager.setUInt(
                        (ParamIndex)i,
                        value.toInt()
                    );
                    break;

                case TYPE_INT8:
                case TYPE_INT16:
                case TYPE_INT32:
                    globalMQTTModule->paramManager.setInt(
                        (ParamIndex)i,
                        value.toInt()
                    );
                    break;

                case TYPE_FLOAT:
                    globalMQTTModule->paramManager.setFloat(
                        (ParamIndex)i,
                        value.toFloat()
                    );
                    break;

                case TYPE_STRING:
                case TYPE_PASSWORD:
                    globalMQTTModule->paramManager.setString(
                        (ParamIndex)i,
                        value.c_str()
                    );
                    break;

                default:
                    break;
            }

            // Re-publish updated value
            globalMQTTModule->publishParam((ParamIndex)i);

            break;
        }
    }
}