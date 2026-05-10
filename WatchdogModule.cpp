// WatchdogModule.cpp

#include "WatchdogModule.h"
#include <WiFi.h>
#include <esp_system.h>

WatchdogModule::WatchdogModule(ParamManager& pm)
    : BaseModule("Watchdog"),
      paramManager(pm),
      lastFeedTime(0),
      lastHealthReport(0),
      failureCount(0) {}

bool WatchdogModule::init() {
    if (!paramManager.getBool(PARAM_WATCHDOG_ENABLED)) {
        initialized = false;
        return false;
    }

    lastFeedTime = millis();
    lastHealthReport = millis();

    initialized = true;
    return true;
}

void WatchdogModule::start() {
    enabled = true;
}

void WatchdogModule::stop() {
    enabled = false;
}

void WatchdogModule::loop() {
    if (!enabled || !paramManager.getBool(PARAM_WATCHDOG_ENABLED)) {
        return;
    }

    feed();
    checkTimeout();

    unsigned long interval = paramManager.getUInt(PARAM_HEALTH_REPORT_INTERVAL_MS);

    if (millis() - lastHealthReport >= interval) {
        lastHealthReport = millis();
        reportHealth();
    }
}

void WatchdogModule::feed() {
    lastFeedTime = millis();
}

void WatchdogModule::checkTimeout() {
    unsigned long timeout = paramManager.getUInt(PARAM_WATCHDOG_TIMEOUT_MS);

    if (millis() - lastFeedTime > timeout) {
        failureCount++;

        Serial.printf("[WATCHDOG] Timeout detected (%u/%u)\n",
                      failureCount,
                      paramManager.getUInt(PARAM_WATCHDOG_MAX_FAILURES));

        lastFeedTime = millis();

        if (failureCount >= paramManager.getUInt(PARAM_WATCHDOG_MAX_FAILURES)) {
            triggerRecovery();
        }
    }
}

void WatchdogModule::triggerRecovery() {
    Serial.println("[WATCHDOG] Critical failure threshold reached.");

    if (!paramManager.getBool(PARAM_AUTO_REBOOT_ENABLED)) {
        Serial.println("[WATCHDOG] Auto reboot disabled.");
        return;
    }

    Serial.println("[WATCHDOG] Rebooting ESP...");
    delay(1000);
    ESP.restart();
}

void WatchdogModule::reportHealth() {
    Serial.println("========== HEALTH REPORT ==========");
    Serial.printf("Uptime: %lu sec\n", millis() / 1000);
    Serial.printf("Free Heap: %u bytes\n", ESP.getFreeHeap());
    Serial.printf("Min Free Heap: %u bytes\n", ESP.getMinFreeHeap());
    Serial.printf("WiFi Connected: %s\n",
                  WiFi.status() == WL_CONNECTED ? "YES" : "NO");

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
        Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
    }

    Serial.printf("Failure Count: %u\n", failureCount);
    Serial.printf("Reset Reason: %d\n", esp_reset_reason());
    Serial.println("===================================");
}