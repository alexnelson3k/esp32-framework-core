//WiFiManagerModule.h

#ifndef WIFI_MANAGER_MODULE_H
#define WIFI_MANAGER_MODULE_H

#include <Arduino.h>
#include <WiFi.h>
#include "ParamManager.h"
#include "BaseModule.h"

class WiFiManagerModule : public BaseModule {
private:
    ParamManager& paramManager;

    bool apMode;
    bool connectionInProgress;

    unsigned long connectStartTime;
    unsigned long lastReconnectAttempt;

    uint8_t retryCount;

    void connectSTA();
    void startAP();
    void disconnect();

    void processConnection();
    void resetConnectionState();

public:
    WiFiManagerModule(ParamManager& pm);

    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;

    void process();

    bool isConnected();
    bool isAPMode();

    String getIP();
    String getSSID();
    int32_t getRSSI();
};

#endif