#include "BuildConfig.h"
#include "Functions.h"
#include "ParamStorageManager.h"
#include "ParamManager.h"
#include "ModuleManager.h"
#include "EventBus.h"
#include "TimeManager.h"
#include "LoggerModule.h"

#if ENABLE_WATCHDOG_MODULE == YES
#include "WatchdogModule.h"
#endif

#if ENABLE_WIFI_MODULE == YES
#include "WiFiManagerModule.h"
#endif

#if ENABLE_COMMANDPROCESSOR_MODULE == YES
#include "CommandProcessor.h"
#endif

#if ENABLE_TELNET_MODULE == YES
#include "TelnetModule.h"
#endif

#if ENABLE_MQTT_MODULE == YES
#include "MQTTModule.h"
#endif

#if ENABLE_API_MODULE == YES
#include "APIModule.h"
#endif

// ============================================================
// Global Core Objects
// ============================================================
ParamStorageManager storageManager;
ParamManager paramManager(storageManager);
TimeManager timeManager(paramManager);
LoggerModule loggerModule(paramManager, timeManager);
ModuleManager moduleManager;
EventBus eventBus;

#if ENABLE_WATCHDOG_MODULE == YES
WatchdogModule watchdogModule(paramManager);
#endif

#if ENABLE_WIFI_MODULE  == YES
WiFiManagerModule wifiManager(paramManager);
#endif

#if ENABLE_COMMANDPROCESSOR_MODULE == YES
CommandProcessor commandProcessor(paramManager, storageManager);
#endif

#if ENABLE_TELNET_MODULE == YES
TelnetModule telnetModule(paramManager, commandProcessor);
#endif

#if ENABLE_MQTT_MODULE == YES
MQTTModule mqttModule(paramManager);
#endif

#if ENABLE_API_MODULE == YES
APIModule apiModule(paramManager, storageManager);
#endif



// ============================================================
// Module Registration
// ============================================================

void registerModules() {

#if ENABLE_WATCHDOG_MODULE == YES
    moduleManager.registerModule(&watchdogModule);
#endif

#if ENABLE_WIFI_MODULE == YES
    moduleManager.registerModule(&wifiManager);
    loggerModule.log(LOG_INFO, "SYSTEM", "Registering WiFi module");
#endif

#if ENABLE_COMMANDPROCESSOR_MODULE == YES
    moduleManager.registerModule(&commandProcessor);
#endif

#if ENABLE_TELNET_MODULE == YES
    moduleManager.registerModule(&telnetModule);
#endif

#if ENABLE_MQTT_MODULE == YES
    moduleManager.registerModule(&mqttModule);
#endif

#if ENABLE_API_MODULE == YES
    moduleManager.registerModule(&apiModule);
#endif

}

// ============================================================
// Arduino Setup
// ============================================================

void setup() {
    initializeSystem();
}

// ============================================================
// Arduino Main Loop
// ============================================================

void loop() {
    loggerModule.loop();
    timeManager.loop();

    unsigned long start = loggerModule.profileStart();
    processCoreTasks();
    loggerModule.profileEnd("CoreTasks", start);
}