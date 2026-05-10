#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <Arduino.h>
#include <vector>
#include "BaseModule.h"

class ModuleManager {
public:
    ModuleManager();

    bool registerModule(BaseModule* module);

    bool initModules();
    void startModules();
    void stopModules();
    void loopModules();

    bool enableModule(const char* moduleName);
    bool disableModule(const char* moduleName);
    bool restartModule(const char* moduleName);

    size_t getRegisteredCount() const;

    BaseModule* getModule(const char* moduleName);

    void listModules(Stream& output);

private:
    std::vector<BaseModule*> modules;

    BaseModule* findModule(const char* moduleName);
};

#endif