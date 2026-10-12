#ifndef SYSTEM_EVENT_H
#define SYSTEM_EVENT_H

#include <Arduino.h>
#include "Param_Types.h"

// ============================================================
// Event Types
// ============================================================

enum EventType {
    EVENT_PARAM_CHANGED,
    EVENT_MODULE_STARTED,
    EVENT_MODULE_STOPPED,
    EVENT_SENSOR_UPDATED,
    EVENT_NETWORK_CONNECTED,
    EVENT_NETWORK_DISCONNECTED,
    EVENT_ERROR,
    EVENT_WARNING,
    EVENT_USER_ACTION,
    EVENT_SYSTEM_BOOT,
    EVENT_SYSTEM_SHUTDOWN
};

// ============================================================
// Event Priority
// ============================================================

enum EventPriority {
    PRIORITY_LOW,
    PRIORITY_NORMAL,
    PRIORITY_HIGH,
    PRIORITY_CRITICAL
};

// ============================================================
// Universal System Event
// ============================================================

struct SystemEvent {
    EventType type;
    EventPriority priority;

    unsigned long timestamp;

    ParamIndex paramIndex;

    const char* sourceModule;

    String message;

    union {
        float floatValue;
        int32_t intValue;
        uint32_t uintValue;
        bool boolValue;
    } data;
};

#endif