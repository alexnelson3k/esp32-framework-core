#ifndef BASE_MODULE_H
#define BASE_MODULE_H

#include <Arduino.h>

class BaseModule {
public:
    BaseModule(const char* moduleName)
        : name(moduleName), enabled(false), initialized(false) {}

    virtual ~BaseModule() = default;

    virtual bool init() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void loop() = 0;

    virtual bool checkDependencies() { return true; }

    const char* getName() const {
        return name;
    }

    bool isEnabled() const {
        return enabled;
    }

    bool isInitialized() const {
        return initialized;
    }

    void setEnabled(bool state) {
        enabled = state;
    }

protected:
    const char* name;
    bool enabled;
    bool initialized;
};

#endif