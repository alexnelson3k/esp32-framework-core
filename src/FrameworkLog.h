#ifndef FRAMEWORK_LOG_H
#define FRAMEWORK_LOG_H

#include "LoggerModule.h"

// Guard before evaluating arguments; disabled DEBUG logs must do no formatting work.
#define FW_LOG_AT(level, module, ...) do { \
    if (globalLogger && globalLogger->shouldLog(level)) \
        globalLogger->logf(level, module, __VA_ARGS__); \
} while (0)
#define FW_LOG_ERROR(module, ...) FW_LOG_AT(LOG_ERROR, module, __VA_ARGS__)
#define FW_LOG_WARN(module, ...) FW_LOG_AT(LOG_WARN, module, __VA_ARGS__)
#define FW_LOG_INFO(module, ...) FW_LOG_AT(LOG_INFO, module, __VA_ARGS__)
#define FW_LOG_DEBUG(module, ...) FW_LOG_AT(LOG_DEBUG, module, __VA_ARGS__)

// Main-loop call sites only; never log from ISRs or WiFi/ESP-NOW callbacks.
#define FW_LOG_EVERY(level, intervalMs, module, ...) do { \
    static bool fwLogged = false; \
    static unsigned long fwLastLog = 0; \
    if (globalLogger && globalLogger->shouldLog(level)) { \
        unsigned long fwNow = millis(); \
        if (!fwLogged || fwNow - fwLastLog >= (intervalMs)) { \
            fwLogged = true; fwLastLog = fwNow; \
            globalLogger->logf(level, module, __VA_ARGS__); \
        } \
    } \
} while (0)

#endif
