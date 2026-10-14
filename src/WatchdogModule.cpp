// WatchdogModule.cpp

#include "WatchdogModule.h"
#include "FrameworkLog.h"
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

        FW_LOG_WARN("Watchdog", "Timeout detected (%u/%u)",
                      failureCount,
                      paramManager.getUInt(PARAM_WATCHDOG_MAX_FAILURES));

        lastFeedTime = millis();

        if (failureCount >= paramManager.getUInt(PARAM_WATCHDOG_MAX_FAILURES)) {
            triggerRecovery();
        }
    }
}

void WatchdogModule::triggerRecovery() {
    FW_LOG_ERROR("Watchdog", "Critical failure threshold reached");

    if (!paramManager.getBool(PARAM_AUTO_REBOOT_ENABLED)) {
        FW_LOG_WARN("Watchdog", "Auto reboot disabled");
        return;
    }

    FW_LOG_ERROR("Watchdog", "Rebooting after failure threshold");
    delay(1000);
    ESP.restart();
}

void WatchdogModule::reportHealth() {
    FW_LOG_DEBUG("Health", "Uptime=%lu s heap=%u min_heap=%u WiFi=%s failures=%u reset=%d",
                 millis() / 1000, ESP.getFreeHeap(), ESP.getMinFreeHeap(),
                 WiFi.status() == WL_CONNECTED ? "connected" : "disconnected",
                 failureCount, static_cast<int>(esp_reset_reason()));
}
