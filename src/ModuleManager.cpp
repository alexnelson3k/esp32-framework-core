#include "ModuleManager.h"
#include "LoggerModule.h"
#include "FrameworkLog.h"

ModuleManager::ModuleManager() {}

bool ModuleManager::registerModule(BaseModule* module) {
    if (!module) return false;

    modules.push_back(module);
    FW_LOG_DEBUG("Modules", "Registered: %s", module->getName());
    return true;
}

BaseModule* ModuleManager::findModule(const char* moduleName) {
    for (BaseModule* module : modules) {
        if (strcmp(module->getName(), moduleName) == 0) {
            return module;
        }
    }

    return nullptr;
}

BaseModule* ModuleManager::getModule(const char* moduleName) {
    return findModule(moduleName);
}

bool ModuleManager::initModules() {
    for (BaseModule* module : modules) {
        if (!module->checkDependencies()) {
            FW_LOG_WARN("Modules", "Dependencies unavailable: %s", module->getName());
            continue;
        }

        if (!module->init()) {
            FW_LOG_ERROR("Modules", "Initialization failed: %s", module->getName());
            return false;
        }

        module->setEnabled(true);
        FW_LOG_INFO("Modules", "Initialized: %s", module->getName());
    }

    return true;
}

void ModuleManager::startModules() {
    for (BaseModule* module : modules) {
        if (module->isEnabled()) {
            module->start();
            FW_LOG_DEBUG("Modules", "Start requested: %s", module->getName());
        }
    }
}

void ModuleManager::stopModules() {
    for (BaseModule* module : modules) {
        if (module->isEnabled()) {
            module->stop();
            module->setEnabled(false);
            FW_LOG_INFO("Modules", "Stopped: %s", module->getName());
        }
    }
}

void ModuleManager::loopModules() {
    for (BaseModule* module : modules) {
        if (module->isEnabled()) {
            LoggerModule* logger = globalLogger;
            unsigned long start = logger ? logger->timeDebugStart() : 0;
            module->loop();
            if (logger) {
                logger->timeDebugModule(module->getName(), start);
            }
        }
    }
}

bool ModuleManager::enableModule(const char* moduleName) {
    FW_LOG_INFO("Modules", "Enable requested: %s", moduleName);
    BaseModule* module = findModule(moduleName);
    if (!module) return false;

    if (!module->isInitialized()) {
        if (!module->init()) {
            return false;
        }
    }

    module->setEnabled(true);
    module->start();

    return true;
}

bool ModuleManager::disableModule(const char* moduleName) {
    FW_LOG_INFO("Modules", "Disable requested: %s", moduleName);
    BaseModule* module = findModule(moduleName);
    if (!module) return false;

    if (module->isEnabled()) {
        module->stop();
        module->setEnabled(false);
    }

    return true;
}

bool ModuleManager::restartModule(const char* moduleName) {
    FW_LOG_INFO("Modules", "Restart requested: %s", moduleName);
    BaseModule* module = findModule(moduleName);
    if (!module) return false;

    if (module->isEnabled()) {
        module->stop();
    }

    if (!module->isInitialized()) {
        if (!module->init()) {
            return false;
        }
    }

    module->setEnabled(true);
    module->start();

    return true;
}

size_t ModuleManager::getRegisteredCount() const {
    return modules.size();
}

void ModuleManager::listModules(Stream& output) {
    output.println("=== Registered Modules ===");

    for (BaseModule* module : modules) {
        output.print(module->getName());
        output.print(" | Init: ");
        output.print(module->isInitialized() ? "YES" : "NO");
        output.print(" | Enabled: ");
        output.println(module->isEnabled() ? "YES" : "NO");
    }
}

