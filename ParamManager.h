#ifndef PARAM_MANAGER_H
#define PARAM_MANAGER_H

#include <Arduino.h>
#include <vector>
#include "Param_Types.h"
#include "ParamStorageManager.h"

class ParamManager {
public:
    explicit ParamManager(ParamStorageManager& storage);

    bool init();

    float getFloat(ParamIndex index);
    bool setFloat(ParamIndex index, float value);

    uint32_t getUInt(ParamIndex index);
    bool setUInt(ParamIndex index, uint32_t value);

    int32_t getInt(ParamIndex index);
    bool setInt(ParamIndex index, int32_t value);

    bool getBool(ParamIndex index);
    bool setBool(ParamIndex index, bool value);

    const char* getString(ParamIndex index);
    bool setString(ParamIndex index, const char* value);

    uint8_t getEnum(ParamIndex index);
    bool setEnum(ParamIndex index, uint8_t value);

    bool hasTag(ParamIndex index, uint32_t tag);
    std::vector<ParamIndex> getParamsByTag(uint32_t tag);

    bool isAccessible(ParamIndex index, uint16_t accessFlag);

    String getDisplayValue(ParamIndex index);

    void resetParam(ParamIndex index);
    void resetAll();

private:
    ParamStorageManager& storageManager;

    template<typename T>
    bool validateRange(T value, T minVal, T maxVal);
};

#endif