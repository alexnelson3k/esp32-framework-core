#ifndef ESP32_FRAMEWORK_H
#define ESP32_FRAMEWORK_H

#include "BuildConfig.h"
#include "Functions.h"
#include "Param_Builders.h"
#include "ParamManager.h"
#include "ParamStorageManager.h"
#include "ModuleManager.h"
#include "EventBus.h"
#include "TimeManager.h"
#include "LoggerModule.h"
#include "WatchdogModule.h"
#include "WiFiManagerModule.h"
#include "ESPNowModule.h"
#include "CommandProcessor.h"
#include "TelnetModule.h"
#include "MQTTModule.h"
#include "APIModule.h"
#include "SyslogModule.h"

extern ParamStorageManager storageManager;
extern ParamManager paramManager;
extern ModuleManager moduleManager;
extern LoggerModule loggerModule;
extern TimeManager timeManager;
extern EventBus eventBus;

bool registerProjectModule(BaseModule* module);
void frameworkConfigureTLS(const char* certificate, const char* privateKey);
void frameworkBegin();
void frameworkLoop();

#endif
