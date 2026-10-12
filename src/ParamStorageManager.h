// ============================================================
// ParamStorageManager.h
// ============================================================

#ifndef PARAM_STORAGE_MANAGER_H
#define PARAM_STORAGE_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include <vector>
#include "Param_Types.h"

#define PARAM_STORAGE_MAGIC 0xDEADBEEF
#define PARAM_REGISTRY_VERSION 1
#define PARAM_SCHEMA_ID "FW_SCHEMA_V1"

struct ParamStorageHeader {
    uint32_t magic;
    uint16_t version;
    uint32_t registryChecksum;
};

class ParamStorageManager {
private:
    Preferences preferences;

    std::vector<uint8_t> dirtyFlags;
    unsigned long lastSaveTime;

    bool validateStorageHeader();
    void saveStorageHeader();
    void resetToFactoryDefaults();

public:
    ParamStorageManager();

    uint32_t calculateRegistryChecksum();

    bool init();

    void scheduleSave(ParamIndex index);
    void process();

    bool saveParam(ParamIndex index);
    bool loadParam(ParamIndex index);

    void saveAll();
    void loadAll();

    void factoryReset();
};

#endif
