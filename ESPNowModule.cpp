#include "ESPNowModule.h"

#include <esp_wifi.h>
#include <stdio.h>

static const uint32_t ESPNOW_PACKET_MAGIC = 0x4553504E; // ESPN
static const uint8_t ESPNOW_PACKET_VERSION = 1;

ESPNowModule* ESPNowModule::instance = nullptr;

ESPNowModule::ESPNowModule(ParamManager& pm)
    : BaseModule("ESPNow"),
      paramManager(pm),
      espNowReady(false),
      peerConfigured(false),
      lastHeartbeat(0),
      sentCount(0),
      failedCount(0),
      receivedCount(0) {

    memset(peerMac, 0, sizeof(peerMac));
    instance = this;
}

bool ESPNowModule::init() {
    initialized = true;

    if (!paramManager.getBool(PARAM_ESPNOW_ENABLED)) {
        return true;
    }

    if (WiFi.getMode() == WIFI_OFF) {
        WiFi.mode(WIFI_STA);
    }

    if (!configureRadioChannel()) {
        espNowReady = false;
        return true;
    }

    if (esp_now_init() != ESP_OK) {
        espNowReady = false;
        return true;
    }

    espNowReady = true;
    esp_now_register_send_cb(onSend);
    esp_now_register_recv_cb(onReceive);

    peerConfigured = configurePeer();
    return true;
}

void ESPNowModule::start() {
    enabled = paramManager.getBool(PARAM_ESPNOW_ENABLED) && espNowReady;
}

void ESPNowModule::stop() {
    enabled = false;

    if (espNowReady) {
        esp_now_deinit();
        espNowReady = false;
        peerConfigured = false;
    }
}

void ESPNowModule::loop() {
    bool shouldRun = paramManager.getBool(PARAM_ESPNOW_ENABLED);

    if (!shouldRun) {
        if (enabled || espNowReady) {
            stop();
        }
        return;
    }

    if (!espNowReady) {
        init();
        start();
    }

    if (!enabled || !espNowReady) return;

    if (!peerConfigured) {
        peerConfigured = configurePeer();
        if (!peerConfigured) return;
    }

    uint32_t interval = paramManager.getUInt(PARAM_ESPNOW_SEND_INTERVAL_MS);
    if (interval == 0) return;

    unsigned long now = millis();
    if (now - lastHeartbeat >= interval) {
        lastHeartbeat = now;
        sendHeartbeat();
    }
}

bool ESPNowModule::configureRadioChannel() {
    uint8_t channel = paramManager.getUInt(PARAM_ESPNOW_CHANNEL);

    if (channel == 0) {
        return true;
    }

    if (channel > 14) {
        return false;
    }

    return esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE) == ESP_OK;
}

bool ESPNowModule::configurePeer() {
    const char* macText = paramManager.getString(PARAM_ESPNOW_PEER_MAC);

    if (!parseMacAddress(macText, peerMac)) {
        return false;
    }

    if (esp_now_is_peer_exist(peerMac)) {
        return true;
    }

    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, peerMac, sizeof(peerMac));
    peerInfo.channel = paramManager.getUInt(PARAM_ESPNOW_CHANNEL);
    peerInfo.encrypt = false;

    return esp_now_add_peer(&peerInfo) == ESP_OK;
}

bool ESPNowModule::parseMacAddress(const char* text, uint8_t* mac) {
    if (!text || strlen(text) < 17) return false;

    unsigned int values[6];
    int parsed = sscanf(
        text,
        "%x:%x:%x:%x:%x:%x",
        &values[0],
        &values[1],
        &values[2],
        &values[3],
        &values[4],
        &values[5]
    );

    if (parsed != 6) return false;

    for (uint8_t i = 0; i < 6; i++) {
        if (values[i] > 0xFF) return false;
        mac[i] = (uint8_t)values[i];
    }

    return true;
}

void ESPNowModule::sendHeartbeat() {
    ESPNowStatusPacket packet;
    packet.magic = ESPNOW_PACKET_MAGIC;
    packet.version = ESPNOW_PACKET_VERSION;
    packet.uptimeSeconds = millis() / 1000;
    packet.freeHeap = ESP.getFreeHeap();
    packet.wifiRssi = WiFi.RSSI();

    esp_err_t result = esp_now_send(
        peerMac,
        reinterpret_cast<const uint8_t*>(&packet),
        sizeof(packet)
    );

    if (result != ESP_OK) {
        failedCount++;
    }
}

void ESPNowModule::onSend(const uint8_t*, esp_now_send_status_t status) {
    if (!instance) return;

    if (status == ESP_NOW_SEND_SUCCESS) {
        instance->sentCount++;
    } else {
        instance->failedCount++;
    }
}

void ESPNowModule::onReceive(const esp_now_recv_info_t*, const uint8_t*, int) {
    if (!instance) return;
    instance->receivedCount++;
}

bool ESPNowModule::isReady() const {
    return enabled && espNowReady && peerConfigured;
}

uint32_t ESPNowModule::getSentCount() const {
    return sentCount;
}

uint32_t ESPNowModule::getFailedCount() const {
    return failedCount;
}

uint32_t ESPNowModule::getReceivedCount() const {
    return receivedCount;
}
