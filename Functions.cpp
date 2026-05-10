#include "Functions.h"
#include "BuildConfig.h"
#include "ParamManager.h"
#include "ParamStorageManager.h"
#include "ModuleManager.h"
#include "EventBus.h"
#include "LoggerModule.h"
#include "TimeManager.h"

extern LoggerModule loggerModule;
extern TimeManager timeManager;
extern ModuleManager moduleManager;
extern ParamStorageManager storageManager;
extern ParamManager paramManager;
extern EventBus eventBus;

void initializeSystem() {
    Serial.begin(DEFAULT_SERIAL_BAUD);

    storageManager.init();
    paramManager.init();

    loggerModule.init();
    loggerModule.start();

    loggerModule.log(LOG_INFO, "SYSTEM", "Framework boot started");

    registerModules();
    
    moduleManager.initModules();
    moduleManager.startModules();

    timeManager.init();
    timeManager.start();

    loggerModule.log(LOG_INFO, "SYSTEM", "System initialization complete");
}

void processCoreTasks() {
    moduleManager.loopModules();
    eventBus.process();
    storageManager.process();
}

String formatUptime(unsigned long ms) {
    unsigned long seconds = ms / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02lu:%02lu:%02lu",
             hours,
             minutes % 60,
             seconds % 60);

    return String(buffer);
}

String boolToString(bool value) {
    return value ? "true" : "false";
}

void safeRestart() {
    ESP.restart();
}