// ============================================================
// LoggerModule.h
// Centralized logging + NTP timestamps + optional execution timing
// ============================================================

#ifndef LOGGER_MODULE_H
#define LOGGER_MODULE_H

#include <Arduino.h>
#include <WiFiUdp.h>

#include "BaseModule.h"
#include "ParamManager.h"
#include "TimeManager.h"

// ============================================================
// Log Levels
// ============================================================

enum LogLevel {
    LOG_ERROR = 0,
    LOG_WARN  = 1,
    LOG_INFO  = 2,
    LOG_DEBUG = 3
};

// ============================================================
// Logger Module
// ============================================================

class LoggerModule : public BaseModule {
private:
    ParamManager& paramManager;

    bool timeDebugEnabled;

    String getTimestamp();
    const char* levelToString(LogLevel level);

    TimeManager& timeManager;

public:
    LoggerModule(ParamManager& pm, TimeManager& tm);

    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;

    void log(LogLevel level, const char* module, const String& message);
    void logf(LogLevel level, const char* module, const char* format, ...);

    // Timing diagnostics
    unsigned long timeDebugStart();
    void timeDebugModule(const char* moduleName, unsigned long startMicros);
    void timeDebugLoop(unsigned long startMicros);
};

// ============================================================
// Global logger access
// ============================================================

extern LoggerModule* globalLogger;

#endif
