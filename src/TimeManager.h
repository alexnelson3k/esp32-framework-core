// ============================================================
// TimeManager.h
// Dedicated system time / NTP / timezone manager
// ============================================================

#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

#include "BaseModule.h"
#include "ParamManager.h"

class TimeManager : public BaseModule {
private:
    ParamManager& paramManager;

    bool timeValid;
    unsigned long lastSync;
    unsigned long syncIntervalMs;

    void syncTimeInternal();

public:
    TimeManager(ParamManager& pm);

    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;

    bool isTimeValid();

    String getFormattedTime();
    String getISOTime();
    uint32_t getEpoch();

    void forceSync();
};

extern TimeManager* globalTimeManager;

#endif