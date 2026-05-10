// ============================================================
// LoggerModule.cpp
// ============================================================

#include "LoggerModule.h"
#include <stdarg.h>

LoggerModule* globalLogger = nullptr;

// ============================================================
// Constructor
// ============================================================

LoggerModule::LoggerModule(
    ParamManager& pm,
    TimeManager& tm
)
    : BaseModule("Logger"),
      paramManager(pm),
      timeManager(tm),
      profilingEnabled(false) {

    globalLogger = this;
}

// ============================================================
// Init
// ============================================================

bool LoggerModule::init() {

    initialized = true;
    enabled = true;

    profilingEnabled =
        paramManager.getBool(PARAM_PROFILING_ENABLED);

    log(
        LOG_INFO,
        "Logger",
        "Logger module initialized"
    );

    return true;
}

// ============================================================
// Start / Stop
// ============================================================

void LoggerModule::start() {
    enabled = true;
}

void LoggerModule::stop() {
    enabled = false;
}

// ============================================================
// Loop
// ============================================================

void LoggerModule::loop() {

    if (!enabled) return;

    profilingEnabled =
        paramManager.getBool(PARAM_PROFILING_ENABLED);

    // Future:
    // Syslog
    // MQTT logs
    // Telnet
    // API buffer
}
// ============================================================
// Timestamp
// ============================================================

String LoggerModule::getTimestamp() {

    if (!paramManager.getBool(PARAM_LOG_TIMESTAMP_ENABLED)) {
        return String(millis());
    }

    return timeManager.getFormattedTime();
}

// ============================================================
// Level Strings
// ============================================================

const char* LoggerModule::levelToString(LogLevel level) {
    switch (level) {
        case LOG_ERROR: return "ERROR";
        case LOG_WARN:  return "WARN";
        case LOG_INFO:  return "INFO";
        case LOG_DEBUG: return "DEBUG";
        default:        return "UNK";
    }
}

// ============================================================
// Core Logging
// ============================================================

void LoggerModule::log(LogLevel level, const char* module, const String& message) {

    uint8_t configuredLevel = paramManager.getUInt(PARAM_LOG_LEVEL);

    if (level > configuredLevel) return;

    String output =
        "[" + getTimestamp() + "]" +
        "[" + String(levelToString(level)) + "]" +
        "[" + String(module) + "] " +
        message;

    // Serial output
    if (paramManager.getBool(PARAM_LOG_SERIAL_ENABLED)) {
        Serial.println(output);
    }

    // Future:
    // Telnet
    // MQTT
    // Syslog
    // API buffer
}

// ============================================================
// printf-style logging
// ============================================================

void LoggerModule::logf(LogLevel level, const char* module, const char* format, ...) {
    char buffer[256];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    log(level, module, String(buffer));
}

// ============================================================
// Profiling
// ============================================================

unsigned long LoggerModule::profileStart() {
    if (!profilingEnabled) return 0;
    return micros();
}

void LoggerModule::profileEnd(
    const char* moduleName,
    unsigned long startMicros
) {

    if (!profilingEnabled || startMicros == 0) return;

    unsigned long elapsed = micros() - startMicros;

    uint32_t warnThreshold =
        paramManager.getUInt(PARAM_PROFILING_WARN_THRESHOLD_US);

    if (elapsed > warnThreshold) {

        logf(
            LOG_WARN,
            "Profiler",
            "%s loop execution time: %lu us",
            moduleName,
            elapsed
        );
    }
}