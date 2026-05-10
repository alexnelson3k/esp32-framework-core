//WiFiManagerModule.cpp

#include "WiFiManagerModule.h"

WiFiManagerModule::WiFiManagerModule(ParamManager& pm)
    : BaseModule("WiFiManager"),
      paramManager(pm),
      apMode(false),
      connectionInProgress(false),
      connectStartTime(0),
      lastReconnectAttempt(0),
      retryCount(0) {}

bool WiFiManagerModule::init() {
    if (!paramManager.getBool(PARAM_WIFI_ENABLED)) {
        WiFi.mode(WIFI_OFF);
        initialized = false;
        return false;
    }
    
    Serial.println("[DEBUG] WiFiManager init called");
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(false);

    resetConnectionState();
    connectSTA();

    initialized = true;
    return true;
}

void WiFiManagerModule::start() {
    enabled = true;
}

void WiFiManagerModule::stop() {
    disconnect();
    enabled = false;
}

void WiFiManagerModule::loop() {
    if (enabled) {
        process();
    }
}

void WiFiManagerModule::resetConnectionState() {
    connectionInProgress = false;
    connectStartTime = 0;
    retryCount = 0;
}

void WiFiManagerModule::connectSTA() {
    String ssid = paramManager.getString(PARAM_WIFI_SSID);
    String password = paramManager.getString(PARAM_WIFI_PASSWORD);

    if (ssid.isEmpty()) {
        startAP();
        return;
    }

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    connectionInProgress = true;
    connectStartTime = millis();
}

void WiFiManagerModule::processConnection() {
    if (!connectionInProgress) return;

    uint32_t timeoutMs = paramManager.getUInt(PARAM_WIFI_TIMEOUT) * 1000;
    uint32_t maxRetries = paramManager.getUInt(PARAM_WIFI_RETRIES);

    if (WiFi.status() == WL_CONNECTED) {
        apMode = false;
        connectionInProgress = false;
        retryCount = 0;
        return;
    }

    if (millis() - connectStartTime >= timeoutMs) {
        connectionInProgress = false;
        retryCount++;

        if (retryCount >= maxRetries) {
            startAP();
        }
    }
}

void WiFiManagerModule::startAP() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);

    String apName = "ESP32_Config_" + String((uint32_t)ESP.getEfuseMac(), HEX);

    WiFi.softAP(apName.c_str(), "admin1234");

    apMode = true;
    connectionInProgress = false;
}

void WiFiManagerModule::disconnect() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    resetConnectionState();
}

void WiFiManagerModule::process() {
    if (!paramManager.getBool(PARAM_WIFI_ENABLED)) {
        disconnect();
        return;
    }

    processConnection();

    if (!apMode &&
        !connectionInProgress &&
        WiFi.status() != WL_CONNECTED) {

        uint32_t reconnectInterval = paramManager.getUInt(PARAM_WIFI_RECONNECT_INTERVAL) * 1000;
        unsigned long now = millis();

        if (now - lastReconnectAttempt >= reconnectInterval) {
            lastReconnectAttempt = now;
            connectSTA();
        }
    }
}

bool WiFiManagerModule::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiManagerModule::isAPMode() {
    return apMode;
}

String WiFiManagerModule::getIP() {
    if (apMode) {
        return WiFi.softAPIP().toString();
    }

    return WiFi.localIP().toString();
}

String WiFiManagerModule::getSSID() {
    if (apMode) {
        return WiFi.softAPSSID();
    }

    return WiFi.SSID();
}

int32_t WiFiManagerModule::getRSSI() {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.RSSI();
    }

    return 0;
}