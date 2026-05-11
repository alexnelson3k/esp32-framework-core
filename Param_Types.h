#ifndef PARAM_TYPES_H
#define PARAM_TYPES_H

#include <Arduino.h>

// ============================================================
// Parameter Types
// ============================================================

enum ParamType {
    TYPE_FLOAT,
    TYPE_DOUBLE,

    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,

    TYPE_INT8,
    TYPE_INT16,
    TYPE_INT32,

    TYPE_BOOL,
    TYPE_STRING,

    TYPE_ENUM,
    TYPE_BITMASK,

    TYPE_IP_ADDRESS,
    TYPE_PASSWORD,
    TYPE_COLOR_RGB,
    TYPE_TIME_SECONDS
};

// ============================================================
// Access / Storage / Security Flags
// ============================================================

enum ParamFlags {
    FLAG_SERIAL_ACCESS      = 0x0001,
    FLAG_DISPLAY_ACCESS     = 0x0002,
    FLAG_ROTARY_ACCESS      = 0x0004,
    FLAG_API_ACCESS         = 0x0008,
    FLAG_MQTT_ACCESS        = 0x0010,
    FLAG_BLUETOOTH_ACCESS   = 0x0020,
    FLAG_SAVE_TO_FLASH      = 0x0040,
    FLAG_SECURED            = 0x0080,
    FLAG_READ_ONLY          = 0x0100,
    FLAG_REQUIRES_REBOOT    = 0x0200,
    FLAG_RUNTIME_ONLY       = 0x0400,
    FLAG_HIDDEN             = 0x0800
};

// ============================================================
// Semantic Tags
// ============================================================

enum ParamTags {
    TAG_SYSTEM         = 0x00000001,
    TAG_NETWORK        = 0x00000002,
    TAG_SECURITY       = 0x00000004,
    TAG_SENSOR         = 0x00000008,
    TAG_CONTROL        = 0x00000010,
    TAG_TEMPERATURE    = 0x00000020,
    TAG_POWER          = 0x00000040,
    TAG_UI             = 0x00000080,
    TAG_AUTOMATION     = 0x00000100,
    TAG_DIAGNOSTICS    = 0x00000200,
    TAG_USER_CONFIG    = 0x00000400,
    TAG_ADVANCED       = 0x00000800
};

// ============================================================
// Enum Option Support
// ============================================================

struct EnumOption {
    uint8_t value;
    const char* label;
};

// ============================================================
// Config Parameter Definition
// ============================================================

struct ConfigParam {
    const char* name;
    const char* description;

    ParamType type;

    uint16_t flags;
    uint32_t tags;

    union {

        struct {
            float value;
            float min;
            float max;
            float step;
            float defaultValue;
        } floatParam;

        struct {
            double value;
            double min;
            double max;
            double step;
            double defaultValue;
        } doubleParam;

        struct {
            uint8_t value;
            uint8_t min;
            uint8_t max;
            uint8_t step;
            uint8_t defaultValue;
        } uint8Param;

        struct {
            uint16_t value;
            uint16_t min;
            uint16_t max;
            uint16_t step;
            uint16_t defaultValue;
        } uint16Param;

        struct {
            uint32_t value;
            uint32_t min;
            uint32_t max;
            uint32_t step;
            uint32_t defaultValue;
        } uint32Param;

        struct {
            int8_t value;
            int8_t min;
            int8_t max;
            int8_t step;
            int8_t defaultValue;
        } int8Param;

        struct {
            int16_t value;
            int16_t min;
            int16_t max;
            int16_t step;
            int16_t defaultValue;
        } int16Param;

        struct {
            int32_t value;
            int32_t min;
            int32_t max;
            int32_t step;
            int32_t defaultValue;
        } int32Param;

        struct {
            bool value;
            bool defaultValue;
        } boolParam;

        struct {
            char value[128];
            size_t maxLength;
            const char* defaultValue;
        } stringParam;

        struct {
            uint8_t value;
            uint8_t defaultValue;
            const EnumOption* options;
            size_t optionCount;
        } enumParam;

        struct {
            uint32_t value;
            uint32_t defaultValue;
        } bitmaskParam;
    };
};

// ============================================================
// Parameter Indexes (Template Examples)
// ============================================================

enum ParamIndex {
    PARAM_DEVICE_NAME,
    PARAM_WIFI_ENABLED,
    PARAM_WIFI_SSID,
    PARAM_WIFI_PASSWORD,
    PARAM_WIFI_TIMEOUT,
    PARAM_WIFI_RETRIES,
    PARAM_WIFI_RECONNECT_INTERVAL,
    PARAM_TELNET_ENABLED,
    PARAM_TELNET_PORT,
    PARAM_MQTT_ENABLED,
    PARAM_MQTT_SERVER,
    PARAM_MQTT_PORT,
    PARAM_MQTT_USERNAME,
    PARAM_MQTT_PASSWORD,
    PARAM_MQTT_CONNECT_TIMEOUT_MS,
    PARAM_MQTT_RECONNECT_INTERVAL_MS,
    PARAM_MQTT_MAX_RETRIES,
    PARAM_MQTT_PUBLISH_INTERVAL_MS,
    PARAM_MQTT_PUBLISH_NETWORK_INTERVAL_MS,
    PARAM_API_ENABLED, 
    PARAM_API_TOKEN,
    PARAM_API_PORT,
    PARAM_WATCHDOG_ENABLED,
    PARAM_WATCHDOG_TIMEOUT_MS,
    PARAM_WATCHDOG_MAX_FAILURES,
    PARAM_HEALTH_REPORT_INTERVAL_MS,
    PARAM_AUTO_REBOOT_ENABLED,
    PARAM_NTP_ENABLED,
    PARAM_NTP_SERVER,
    PARAM_TIMEZONE_OFFSET_MINUTES,
    PARAM_DST_ENABLED,
    PARAM_NTP_RESYNC_INTERVAL_MS,
    PARAM_LOG_LEVEL,
    PARAM_LOG_SERIAL_ENABLED,
    PARAM_LOG_TELNET_ENABLED,
    PARAM_LOG_TIMESTAMP_ENABLED,
    PARAM_PROFILING_ENABLED,
    PARAM_PROFILING_WARN_THRESHOLD_US,
    PARAM_SYSLOG_ENABLED,
    PARAM_SYSLOG_SERVER,
    PARAM_SYSLOG_PORT,
    PARAM_SYSLOG_FACILITY,
    PARAM_SYSLOG_MIN_LEVEL,
    PARAM_TEMP_SETPOINT,
    PARAM_SENSOR_ENABLED,
    PARAM_COUNT
};

// ============================================================
// Global Param Table
// ============================================================

extern ConfigParam system_params[PARAM_COUNT];

// ============================================================
// Helpers
// ============================================================

String getParamDisplayValue(ParamIndex index);

#endif
