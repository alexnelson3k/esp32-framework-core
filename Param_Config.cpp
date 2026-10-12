//Param_Config.cpp

#include "Param_Types.h"
#include "Param_Builders.h"
#include "secrets.h"

// ============================================================
// Enum Definitions
// ============================================================

constexpr EnumOption log_level_options[] = {
    {0, "ERROR"},
    {1, "WARN"},
    {2, "INFO"},
    {3, "DEBUG"}
};

// ============================================================
// Global Parameter Table
// ============================================================

ConfigParam system_params[PARAM_COUNT] = {

    [PARAM_DEVICE_NAME] = stringParam("device_name")
        .description("Device Name")
        .maxLength(64).defaultValue("ESP32_Device")
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_USER_CONFIG).build(),

    [PARAM_WIFI_ENABLED] = boolParam("wifi_enabled")
        .description("Enable WiFi")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_WIFI_SSID] = stringParam("wifi_ssid")
        .description("WiFi SSID")
        .maxLength(64).defaultValue(DEFAULT_SSID)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK | TAG_SECURITY).build(),

    [PARAM_WIFI_PASSWORD] = passwordParam("wifi_password")
        .description("WiFi Password")
        .maxLength(64).defaultValue(DEFAULT_WIFI_PASSWORD)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SECURED).persistent()
        .tags(TAG_NETWORK | TAG_SECURITY).build(),
    
    [PARAM_WIFI_TIMEOUT] = uint16Param("wifi_timeout")
        .description("WiFi connection timeout (seconds)")
        .defaultValue(15).range(3, 120).step(1)
        .access(FLAG_SERIAL_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_WIFI_RETRIES] = uint8Param("wifi_retries")
        .description("WiFi retry attempts before AP fallback")
        .defaultValue(3).range(1, 20).step(1)
        .access(FLAG_SERIAL_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),
    
    [PARAM_WIFI_RECONNECT_INTERVAL] = uint16Param("wifi_reconnect_interval")
        .description("WiFi reconnect interval (seconds)")
        .defaultValue(10).range(5, 300).step(1)
        .access(FLAG_SERIAL_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_TELNET_ENABLED] = boolParam("telnet_enabled")
        .description("Enable Telnet console")
        .defaultValue(false)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_TELNET_PORT] = uint16Param("telnet_port")
        .description("Telnet server port")
        .defaultValue(23).range(1, 65535).step(1)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),
    
    [PARAM_MQTT_ENABLED] = boolParam("mqtt_enabled")
        .description("Enable MQTT module")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_MQTT_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_MQTT_SERVER] = stringParam("mqtt_server")
        .description("MQTT Broker Address")
        .maxLength(128).defaultValue(DEFAULT_MQTT_SERVER)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_MQTT_PORT] = uint16Param("mqtt_port")
        .description("MQTT broker port")
        .defaultValue(1883).range(1, 65535).step(1)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_MQTT_USERNAME] = stringParam("mqtt_username")
        .description("MQTT username")
        .maxLength(128).defaultValue(DEFAULT_MQTT_USERNAME)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SECURITY).build(),

    [PARAM_MQTT_PASSWORD] = passwordParam("mqtt_password")
        .description("MQTT password")
        .maxLength(128).defaultValue(DEFAULT_MQTT_PASSWORD)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS | FLAG_SECURED).persistent()
        .tags(TAG_SECURITY).build(),

    [PARAM_MQTT_CONNECT_TIMEOUT_MS] = uint32Param("mqtt_connect_timeout_ms")
        .description("MQTT connect timeout")
        .defaultValue(5000).range(1000, 60000).step(500)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_MQTT_RECONNECT_INTERVAL_MS] = uint32Param("mqtt_reconnect_interval_ms")
        .description("MQTT reconnect interval")
        .defaultValue(10000).range(1000, 300000).step(1000)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_MQTT_MAX_RETRIES] = uint8Param("mqtt_max_retries")
        .description("MQTT reconnect retries")
        .defaultValue(10).range(1, 255).step(1)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_MQTT_PUBLISH_INTERVAL_MS] = uint32Param("mqtt_publish_interval_ms")
        .description("MQTT publish interval")
        .defaultValue(5000).range(1000, 300000).step(1000)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_MQTT_PUBLISH_NETWORK_INTERVAL_MS] = uint32Param("mqtt_publish_network_interval_ms")
        .description("MQTT network publish interval")
        .defaultValue(60000).range(5000, 3600000).step(1000)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_API_ENABLED] = boolParam("api_enabled")
        .description("Enable REST API HTTPS Module")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_API_TOKEN] = stringParam("api_token")
        .description("API Bearer authentication token")
        .maxLength(64).defaultValue(DEFAULT_API_TOKEN)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_API_PORT] = uint16Param("api_port")
        .description("HTTPS API server port")
        .defaultValue(443).range(1, 65535).step(1)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK).build(),

    [PARAM_WATCHDOG_ENABLED] = boolParam("watchdog_enabled")
        .description("Enable system watchdog")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_WATCHDOG_TIMEOUT_MS] = uint32Param("watchdog_timeout_ms")
        .description("Watchdog timeout in milliseconds")
        .defaultValue(5000).range(1000, 60000).step(500)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_WATCHDOG_MAX_FAILURES] = uint8Param("watchdog_max_failures")
        .description("Maximum watchdog failures before reboot")
        .defaultValue(3).range(1, 10).step(1)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_HEALTH_REPORT_INTERVAL_MS] = uint32Param("health_report_interval_ms")
        .description("Health reporting interval")
        .defaultValue(60000).range(5000, 3600000).step(1000)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_AUTO_REBOOT_ENABLED] = boolParam("auto_reboot_enabled")
        .description("Allow automatic reboot on critical failure")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_NTP_ENABLED] = boolParam("ntp_enabled")
        .description("Enable NTP time synchronization")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_NETWORK).build(),
    
    [PARAM_NTP_SERVER] = stringParam("ntp_server")
        .description("Primary NTP server hostname")
        .maxLength(64).defaultValue("pool.ntp.org")
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_NETWORK).build(),

    [PARAM_TIMEZONE_OFFSET_MINUTES] = int16Param("timezone_offset_minutes")
        .description("Timezone offset from UTC in minutes")
        .defaultValue(0).range(-720, 840).step(15)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_USER_CONFIG).build(),

    [PARAM_DST_ENABLED] = boolParam("dst_enabled")
        .description("Enable daylight saving time adjustment")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_USER_CONFIG).build(),

    [PARAM_NTP_RESYNC_INTERVAL_MS] = uint32Param("ntp_resync_interval_ms")
        .description("NTP synchronization interval in milliseconds")
        .defaultValue(3600000).range(60000, 86400000).step(60000)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_NETWORK).build(),

    [PARAM_LOG_LEVEL] = enumParam("log_level", log_level_options)
        .description("Logging verbosity level")
        .defaultValue(2)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_LOG_SERIAL_ENABLED] = boolParam("log_serial_enabled")
        .description("Enable serial logging")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_LOG_TELNET_ENABLED] = boolParam("log_telnet_enabled")
        .description("Enable Telnet logging")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_LOG_TIMESTAMP_ENABLED] = boolParam("log_timestamp_enabled")
        .description("Enable human-readable timestamps")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_PROFILING_ENABLED] = boolParam("profiling_enabled")
        .description("Enable module loop profiling")
        .defaultValue(false)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_DIAGNOSTICS | TAG_ADVANCED).build(),

    [PARAM_PROFILING_WARN_THRESHOLD_US] = uint32Param("profiling_warn_threshold_us")
        .description("Loop execution warning threshold (microseconds)")
        .defaultValue(5000).range(100, 1000000).step(100)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_DIAGNOSTICS | TAG_ADVANCED).build(),

    [PARAM_LOG_SYSLOG_ENABLED] = boolParam("log_syslog_enabled")
        .description("Enable syslog logging")
        .defaultValue(false)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_NETWORK | TAG_DIAGNOSTICS).build(),

    [PARAM_SYSLOG_SERVER] = stringParam("syslog_server")
        .description("Syslog server hostname or IP address")
        .maxLength(128).defaultValue("")
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_NETWORK | TAG_DIAGNOSTICS).build(),

    [PARAM_SYSLOG_PORT] = uint16Param("syslog_port")
        .description("Syslog UDP server port")
        .defaultValue(514).range(1, 65535).step(1)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_NETWORK | TAG_DIAGNOSTICS).build(),

    [PARAM_SYSLOG_FACILITY] = uint8Param("syslog_facility")
        .description("Syslog facility code")
        .defaultValue(16).range(0, 23).step(1)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS | TAG_ADVANCED).build(),

    [PARAM_SYSLOG_MIN_LEVEL] = enumParam("syslog_min_level", log_level_options)
        .description("Maximum log verbosity sent to syslog")
        .defaultValue(2)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SYSTEM | TAG_DIAGNOSTICS).build(),

    [PARAM_ESPNOW_ENABLED] = boolParam("espnow_enabled")
        .description("Enable ESP-NOW module")
        .defaultValue(false)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK | TAG_ADVANCED).build(),

    [PARAM_ESPNOW_PEER_MAC] = stringParam("espnow_peer_mac")
        .description("ESP-NOW peer MAC address")
        .maxLength(18).defaultValue("FF:FF:FF:FF:FF:FF")
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK | TAG_ADVANCED).build(),

    [PARAM_ESPNOW_CHANNEL] = uint8Param("espnow_channel")
        .description("ESP-NOW WiFi channel, 0 follows current channel")
        .defaultValue(0).range(0, 14).step(1)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK | TAG_ADVANCED).build(),

    [PARAM_ESPNOW_SEND_INTERVAL_MS] = uint32Param("espnow_send_interval_ms")
        .description("ESP-NOW status heartbeat interval")
        .defaultValue(30000).range(1000, 3600000).step(1000)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_NETWORK | TAG_DIAGNOSTICS | TAG_ADVANCED).build(),

    [PARAM_TEMP_SETPOINT] = floatParam("temp_setpoint")
        .description("Temperature Setpoint")
        .defaultValue(22.5f).range(0.0f, 100.0f).step(0.1f)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_CONTROL | TAG_TEMPERATURE).build(),

    [PARAM_SENSOR_ENABLED] = boolParam("sensor_enabled")
        .description("Enable Sensors")
        .defaultValue(true)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_SENSOR).build()

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
