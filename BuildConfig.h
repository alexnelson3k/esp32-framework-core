#ifndef BUILD_CONFIG_H
#define BUILD_CONFIG_H

#define YES 1
#define NO  0

// ============================================================
// Firmware Version
// ============================================================

#define FW_VERSION_MAJOR 0
#define FW_VERSION_MINOR 1
#define FW_VERSION_PATCH 0

// ============================================================
// Core Modules
// ============================================================
#define ENABLE_WATCHDOG_MODULE                YES
#define ENABLE_WIFI_MODULE                    YES
#define ENABLE_ESPNOW_MODULE                  YES
#define ENABLE_COMMANDPROCESSOR_MODULE        YES
#define ENABLE_TELNET_MODULE                  YES
#define ENABLE_MQTT_MODULE                    YES
#define ENABLE_API_MODULE                     YES
#define ENABLE_SYSLOG_MODULE                  YES

//#define ENABLE_DISPLAY_MODULE
//#define ENABLE_SERIAL_MODULE


// ============================================================
// Optional Modules
// ============================================================

// #define ENABLE_BLUETOOTH_MODULE
// #define ENABLE_INFLUXDB_MODULE
// #define ENABLE_OTA_MODULE
// #define ENABLE_RULE_ENGINE

// ============================================================
// Debugging
// ============================================================

#define DEBUG_SERIAL
#define DEBUG_LEVEL 3

// Levels:
// 0 = none
// 1 = errors
// 2 = warnings
// 3 = info
// 4 = verbose

// ============================================================
// Storage
// ============================================================

#define PARAM_STORAGE_NAMESPACE "system_cfg"
#define PARAM_SAVE_DELAY_MS 5000

// ============================================================
// EventBus
// ============================================================

#define EVENT_QUEUE_SIZE 32

// ============================================================
// Hardware
// ============================================================

#define DEFAULT_I2C_SDA 21
#define DEFAULT_I2C_SCL 22

#define DEFAULT_SERIAL_BAUD 115200

// ============================================================
// Tasking
// ============================================================

#define ENABLE_FREERTOS_TASKS
#define CORE_EVENTBUS 1
#define CORE_NETWORK 0


#endif
