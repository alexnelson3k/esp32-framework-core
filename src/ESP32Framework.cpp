#include "ESP32Framework.h"
#include <vector>

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

#if ENABLE_ESPNOW_MODULE == YES
ESPNowModule espNowModule(paramManager);
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

#if ENABLE_SYSLOG_MODULE == YES
SyslogModule syslogModule(paramManager);
#endif



// ============================================================
// Module Registration
// ============================================================

static std::vector<BaseModule*>& projectModules() {
    static std::vector<BaseModule*> modules;
    return modules;
}

bool registerProjectModule(BaseModule* module) {
    if (!module) return false;
    projectModules().push_back(module);
    return true;
}

void registerModules() {

#if ENABLE_WATCHDOG_MODULE == YES
    if (paramManager.getBool(PARAM_WATCHDOG_ENABLED))
        moduleManager.registerModule(&watchdogModule);
#endif

#if ENABLE_WIFI_MODULE == YES
    if (paramManager.getBool(PARAM_WIFI_ENABLED))
        moduleManager.registerModule(&wifiManager);
    loggerModule.log(LOG_INFO, "SYSTEM", "Registering WiFi module");
#endif

#if ENABLE_ESPNOW_MODULE == YES
    moduleManager.registerModule(&espNowModule);
#endif

#if ENABLE_COMMANDPROCESSOR_MODULE == YES
    moduleManager.registerModule(&commandProcessor);
#endif

#if ENABLE_TELNET_MODULE == YES
    moduleManager.registerModule(&telnetModule);
#endif

#if ENABLE_MQTT_MODULE == YES
    if (paramManager.getBool(PARAM_MQTT_ENABLED))
        moduleManager.registerModule(&mqttModule);
#endif

#if ENABLE_API_MODULE == YES
    moduleManager.registerModule(&apiModule);
#endif

#if ENABLE_SYSLOG_MODULE == YES
    moduleManager.registerModule(&syslogModule);
#endif

    for (BaseModule* module : projectModules()) {
        moduleManager.registerModule(module);
    }
}

void frameworkBegin() {
    initializeSystem();
}

void frameworkLoop() {
    unsigned long loopStart = micros();
    loggerModule.loop();
    loggerModule.timeDebugModule("Logger", loopStart);

    unsigned long timeStart = loggerModule.timeDebugStart();
    timeManager.loop();
    loggerModule.timeDebugModule("TimeManager", timeStart);

    processCoreTasks();
    loggerModule.timeDebugLoop(loopStart);
}

void frameworkConfigureTLS(const char* cert, const char* key) {
#if ENABLE_API_MODULE == YES
    apiModule.configureTLS(cert, key);
#else
    (void)cert;
    (void)key;
#endif
}
