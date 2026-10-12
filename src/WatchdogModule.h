// WatchdogModule.h

#ifndef WATCHDOG_MODULE_H
#define WATCHDOG_MODULE_H

#include <Arduino.h>
#include "BaseModule.h"
#include "ParamManager.h"

class WatchdogModule : public BaseModule {
private:
    ParamManager& paramManager;

    unsigned long lastFeedTime;
    unsigned long lastHealthReport;
    uint8_t failureCount;

    void feed();
    void checkTimeout();
    void triggerRecovery();
    void reportHealth();

public:
    WatchdogModule(ParamManager& pm);

    uint32_t getFailureCount() const { return failureCount; }
    
    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;
};

#endif