//Param_Config.cpp

#include "Param_Types.h"

// ============================================================
// Enum Definitions
// ============================================================

const EnumOption log_level_options[] = {
    {0, "ERROR"},
    {1, "WARN"},
    {2, "INFO"},
    {3, "DEBUG"}
};

// ============================================================
// Global Parameter Table
// ============================================================

ConfigParam system_params[PARAM_COUNT] = {

    [PARAM_DEVICE_NAME] = {
        "device_name",
        "Device Name",
        TYPE_STRING,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_USER_CONFIG,
        {.stringParam = {"ESP32_Device", 64, "ESP32_Device"}}
    },

    [PARAM_WIFI_ENABLED] = {
        "wifi_enabled",
        "Enable WiFi",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.boolParam = {true, true}}
    },

    [PARAM_WIFI_SSID] = {
        "wifi_ssid",
        "WiFi SSID",
        TYPE_STRING,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK | TAG_SECURITY,
        {.stringParam = {"MyWiFi", 64, "MyWiFi"}}
    },

    [PARAM_WIFI_PASSWORD] = {
        "wifi_password",
        "WiFi Password",
        TYPE_PASSWORD,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH | FLAG_SECURED,
        TAG_NETWORK | TAG_SECURITY,
        {.stringParam = {"password123", 64, "password123"}}
    },
    
    [PARAM_WIFI_TIMEOUT] = {
        "wifi_timeout",
        "WiFi connection timeout (seconds)",
        TYPE_UINT16,
        FLAG_SAVE_TO_FLASH | FLAG_SERIAL_ACCESS,
        TAG_NETWORK,
        {.uint16Param = { 15, 3, 120, 1, 15 }}
    },

    [PARAM_WIFI_RETRIES] = {
        "wifi_retries",
        "WiFi retry attempts before AP fallback",
        TYPE_UINT8,
        FLAG_SAVE_TO_FLASH | FLAG_SERIAL_ACCESS,
        TAG_NETWORK,
        {.uint8Param = { 3, 1, 20, 1, 3 }}
    },
    
    [PARAM_WIFI_RECONNECT_INTERVAL] = {
        "wifi_reconnect_interval",
        "WiFi reconnect interval (seconds)",
        TYPE_UINT16,
        FLAG_SAVE_TO_FLASH | FLAG_SERIAL_ACCESS,
        TAG_NETWORK,
        {.uint16Param = { 10, 5, 300, 1, 10 }}
    },

    [PARAM_TELNET_ENABLED] = {
        "telnet_enabled",
        "Enable Telnet console",
        TYPE_BOOL,
        FLAG_SAVE_TO_FLASH | FLAG_SERIAL_ACCESS | FLAG_API_ACCESS,
        TAG_NETWORK,
        {.boolParam = { false, false }}
    },

    [PARAM_TELNET_PORT] = {
        "telnet_port",
        "Telnet server port",
        TYPE_UINT16,
        FLAG_SAVE_TO_FLASH | FLAG_SERIAL_ACCESS | FLAG_API_ACCESS,
        TAG_NETWORK,
        {.uint16Param = { 23, 1, 65535, 1, 23 }}
    },
    
    [PARAM_MQTT_ENABLED] = {
        "mqtt_enabled",
        "Enable MQTT module",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH | FLAG_MQTT_ACCESS,
        TAG_NETWORK,
        {.boolParam = { true, true }}
    },

    [PARAM_MQTT_SERVER] = {
        "mqtt_server",
        "MQTT Broker Address",
        TYPE_STRING,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.stringParam = {"broker.local", 128, "broker.local" }}
    },

    [PARAM_MQTT_PORT] = {
        "mqtt_port",
        "MQTT broker port",
        TYPE_UINT16,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.uint16Param = { 1883, 1, 65535, 1, 1883 }}
    },

    [PARAM_MQTT_USERNAME] = {
        "mqtt_username",
        "MQTT username",
        TYPE_STRING,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SECURITY,
        {.stringParam = { "", 128, "" }}
    },

    [PARAM_MQTT_PASSWORD] = {
        "mqtt_password",
        "MQTT password",
        TYPE_PASSWORD,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH | FLAG_SECURED,
        TAG_SECURITY,
        {.stringParam = { "", 128, "" }}
    },

    [PARAM_MQTT_CONNECT_TIMEOUT_MS] = {
        "mqtt_connect_timeout_ms",
        "MQTT connect timeout",
        TYPE_UINT32,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.uint32Param = { 5000, 1000, 60000, 500, 5000 }}
    },

    [PARAM_MQTT_RECONNECT_INTERVAL_MS] = {
        "mqtt_reconnect_interval_ms",
        "MQTT reconnect interval",
        TYPE_UINT32,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.uint32Param = { 10000, 1000, 300000, 1000, 10000 }}
    },

    [PARAM_MQTT_MAX_RETRIES] = {
        "mqtt_max_retries",
        "MQTT reconnect retries",
        TYPE_UINT8,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.uint8Param = { 10, 1, 255, 1, 10 }}
    },

    [PARAM_MQTT_PUBLISH_INTERVAL_MS] = {
        "mqtt_publish_interval_ms",
        "MQTT publish interval",
        TYPE_UINT32,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.uint32Param = { 5000, 1000, 300000, 1000, 5000 }}
    },

    [PARAM_MQTT_PUBLISH_NETWORK_INTERVAL_MS] = {
        "mqtt_publish_network_interval_ms",
        "MQTT network publish interval",
        TYPE_UINT32,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.uint32Param = { 60000, 5000, 3600000, 1000, 60000 }}
    },

    [PARAM_API_ENABLED] = {
        "api_enabled",
        "Enable REST API HTTPS Module",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.boolParam = {true, true}}
    },

    [PARAM_API_TOKEN] = {
        "api_token",
        "API Bearer authentication token",
        TYPE_STRING,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.stringParam = {"ChangeMe123456", 64, "" }}
    },

    [PARAM_API_PORT] = {
        "api_port",
        "HTTPS API server port",
        TYPE_UINT16,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_NETWORK,
        {.uint16Param = {443, 443, 1, 65535 }}
    },

    [PARAM_WATCHDOG_ENABLED] = {
        "watchdog_enabled",
        "Enable system watchdog",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_DIAGNOSTICS,
        {.boolParam = {true, true}}
    },

    [PARAM_WATCHDOG_TIMEOUT_MS] = {
        "watchdog_timeout_ms",
        "Watchdog timeout in milliseconds",
        TYPE_UINT32,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_DIAGNOSTICS,
        {.uint32Param = {5000, 1000, 60000, 500, 5000}}
    },

    [PARAM_WATCHDOG_MAX_FAILURES] = {
        "watchdog_max_failures",
        "Maximum watchdog failures before reboot",
        TYPE_UINT8,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_DIAGNOSTICS,
        {.uint8Param = {3, 1, 10, 1, 3}}
    },

    [PARAM_HEALTH_REPORT_INTERVAL_MS] = {
        "health_report_interval_ms",
        "Health reporting interval",
        TYPE_UINT32,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_DIAGNOSTICS,
        {.uint32Param = {60000, 5000, 3600000, 1000, 60000}}
    },

    [PARAM_AUTO_REBOOT_ENABLED] = {
        "auto_reboot_enabled",
        "Allow automatic reboot on critical failure",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_DIAGNOSTICS,
        {.boolParam = {true, true}}
    },

    [PARAM_NTP_ENABLED] = {
        "ntp_enabled",
        "Enable NTP time synchronization",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_NETWORK,
        {.boolParam = { true, true }}
    },
    
    [PARAM_NTP_SERVER] = {
        "ntp_server",
        "Primary NTP server hostname",
        TYPE_STRING,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_NETWORK,
        {.stringParam = { "pool.ntp.org", 64, "pool.ntp.org" }}
    },

    [PARAM_TIMEZONE_OFFSET_MINUTES] = {
        "timezone_offset_minutes",
        "Timezone offset from UTC in minutes",
        TYPE_INT16,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_USER_CONFIG,
        {.int16Param = { 0, -720, 840, 15, 0 }}
    },

    [PARAM_DST_ENABLED] = {
        "dst_enabled",
        "Enable daylight saving time adjustment",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_USER_CONFIG,
        {.boolParam = { true, true }}
    },

    [PARAM_NTP_RESYNC_INTERVAL_MS] = {
        "ntp_resync_interval_ms",
        "NTP synchronization interval in milliseconds",
        TYPE_UINT32,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_NETWORK,
        {.uint32Param = { 3600000, 60000, 86400000, 60000, 3600000 }}
    },

    [PARAM_LOG_LEVEL] = {
        "log_level",
        "Logging verbosity level",
        TYPE_ENUM,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_DIAGNOSTICS,
        {.enumParam = { 2, 2, log_level_options, 4 }}
    },

    [PARAM_LOG_SERIAL_ENABLED] = {
        "log_serial_enabled",
        "Enable serial logging",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_DIAGNOSTICS,
        {.boolParam = { true, true }}
    },

    [PARAM_LOG_TELNET_ENABLED] = {
        "log_telnet_enabled",
        "Enable Telnet logging",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_DIAGNOSTICS,
        {.boolParam = { true, true }}
    },

    [PARAM_LOG_TIMESTAMP_ENABLED] = {
        "log_timestamp_enabled",
        "Enable human-readable timestamps",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SYSTEM | TAG_DIAGNOSTICS,
        {.boolParam = { true, true }}
    },

    [PARAM_PROFILING_ENABLED] = {
        "profiling_enabled",
        "Enable module loop profiling",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_DIAGNOSTICS | TAG_ADVANCED,
        {.boolParam = { false, false }}
    },

    [PARAM_PROFILING_WARN_THRESHOLD_US] = {
        "profiling_warn_threshold_us",
        "Loop execution warning threshold (microseconds)",
        TYPE_UINT32,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_DIAGNOSTICS | TAG_ADVANCED,
        {.uint32Param = { 5000, 100, 1000000, 100, 5000 }}
    },

    [PARAM_TEMP_SETPOINT] = {
        "temp_setpoint",
        "Temperature Setpoint",
        TYPE_FLOAT,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_CONTROL | TAG_TEMPERATURE,
        {.floatParam = {22.5f, 0.0f, 100.0f, 0.1f, 22.5f}}
    },

    [PARAM_SENSOR_ENABLED] = {
        "sensor_enabled",
        "Enable Sensors",
        TYPE_BOOL,
        FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SAVE_TO_FLASH,
        TAG_SENSOR,
        {.boolParam = {true, true}}
    }

};

String getParamDisplayValue(ParamIndex index) {
    ConfigParam& param = system_params[index];

    switch (param.type) {

        case TYPE_FLOAT:
            return String(param.floatParam.value);

        case TYPE_DOUBLE:
            return String(param.doubleParam.value);

        case TYPE_UINT8:
            return String(param.uint8Param.value);

        case TYPE_UINT16:
            return String(param.uint16Param.value);

        case TYPE_UINT32:
            return String(param.uint32Param.value);

        case TYPE_INT8:
            return String(param.int8Param.value);

        case TYPE_INT16:
            return String(param.int16Param.value);

        case TYPE_INT32:
            return String(param.int32Param.value);

        case TYPE_BOOL:
            return param.boolParam.value ? "true" : "false";

        case TYPE_STRING:
        case TYPE_PASSWORD:
            return String(param.stringParam.value);

        case TYPE_ENUM:
            return String(param.enumParam.options[param.enumParam.value].label);

        case TYPE_BITMASK:
            return String(param.bitmaskParam.value, HEX);

        default:
            return "N/A";
    }
}