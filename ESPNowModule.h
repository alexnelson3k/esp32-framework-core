#ifndef ESPNOW_MODULE_H
#define ESPNOW_MODULE_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "BaseModule.h"
#include "ParamManager.h"

struct ESPNowStatusPacket {
    uint32_t magic;
    uint8_t version;
    uint32_t uptimeSeconds;
    uint32_t freeHeap;
    int32_t wifiRssi;
};

class ESPNowModule : public BaseModule {
private:
    ParamManager& paramManager;

    bool espNowReady;
    bool peerConfigured;
    uint8_t peerMac[6];

    unsigned long lastHeartbeat;
    uint32_t sentCount;
    uint32_t failedCount;
    uint32_t receivedCount;

    bool configureRadioChannel();
    bool configurePeer();
    bool parseMacAddress(const char* text, uint8_t* mac);
    void sendHeartbeat();

    static ESPNowModule* instance;
    static void onSend(const uint8_t* macAddr, esp_now_send_status_t status);
    static void onReceive(const esp_now_recv_info_t* info, const uint8_t* data, int len);

public:
    explicit ESPNowModule(ParamManager& pm);

    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;

    bool isReady() const;
    uint32_t getSentCount() const;
    uint32_t getFailedCount() const;
    uint32_t getReceivedCount() const;
};

#endif
