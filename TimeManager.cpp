// ============================================================
// TimeManager.cpp
// ============================================================

#include "TimeManager.h"

TimeManager* globalTimeManager = nullptr;

// ============================================================
// Constructor
// ============================================================

TimeManager::TimeManager(ParamManager& pm)
    : BaseModule("TimeManager"),
      paramManager(pm),
      timeValid(false),
      lastSync(0),
      syncIntervalMs(3600000) {

    globalTimeManager = this;
}

// ============================================================
// Init
// ============================================================

bool TimeManager::init() {

    if (!paramManager.getBool(PARAM_NTP_ENABLED)) {
        initialized = true;
        enabled = false;
        return true;
    }

    syncIntervalMs =
        paramManager.getUInt(PARAM_NTP_RESYNC_INTERVAL_MS);

    initialized = true;
    enabled = true;

    if (WiFi.isConnected()) {
        syncTimeInternal();
    }

    return true;
}

// ============================================================
// Start / Stop
// ============================================================

void TimeManager::start() {
    enabled = true;
}

void TimeManager::stop() {
    enabled = false;
}

// ============================================================
// Loop
// ============================================================

void TimeManager::loop() {

    if (!enabled) return;

    if (!paramManager.getBool(PARAM_NTP_ENABLED)) return;

    syncIntervalMs =
        paramManager.getUInt(PARAM_NTP_RESYNC_INTERVAL_MS);

    if (!WiFi.isConnected()) return;

    unsigned long now = millis();

    if (now - lastSync >= syncIntervalMs) {
        syncTimeInternal();
        lastSync = now;
    }
}

// ============================================================
// Core Sync
// ============================================================

void TimeManager::syncTimeInternal() {

    const char* ntpServer =
        paramManager.getString(PARAM_NTP_SERVER);

    int timezoneMinutes =
        paramManager.getInt(PARAM_TIMEZONE_OFFSET_MINUTES);

    bool dstEnabled =
        paramManager.getBool(PARAM_DST_ENABLED);

    long gmtOffsetSeconds =
        timezoneMinutes * 60;

    long dstOffsetSeconds =
        dstEnabled ? 3600 : 0;

    configTime(
        gmtOffsetSeconds,
        dstOffsetSeconds,
        ntpServer
    );

    struct tm timeinfo;

    if (getLocalTime(&timeinfo, 10000)) {
        timeValid = true;
    } else {
        timeValid = false;
    }
}

// ============================================================
// Public Sync
// ============================================================

void TimeManager::forceSync() {
    syncTimeInternal();
    lastSync = millis();
}

// ============================================================
// Status
// ============================================================

bool TimeManager::isTimeValid() {
    return timeValid;
}

// ============================================================
// Epoch
// ============================================================

uint32_t TimeManager::getEpoch() {

    time_t now;
    time(&now);

    return (uint32_t)now;
}

// ============================================================
// Human-readable
// ============================================================

String TimeManager::getFormattedTime() {

    if (!timeValid) {
        return String("UPTIME:") + String(millis() / 1000);
    }

    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
        return String("UPTIME:") + String(millis() / 1000);
    }

    char buffer[32];

    strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d %H:%M:%S",
        &timeinfo
    );

    return String(buffer);
}

// ============================================================
// ISO8601
// ============================================================

String TimeManager::getISOTime() {

    if (!timeValid) {
        return "";
    }

    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
        return "";
    }

    char buffer[40];

    strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%dT%H:%M:%S",
        &timeinfo
    );

    return String(buffer);
}