#include "ParamManager.h"
#include <cstring>

ParamManager::ParamManager(ParamStorageManager& storage)
    : storageManager(storage) {}

bool ParamManager::init() {
    return storageManager.init();
}

template<typename T>
bool ParamManager::validateRange(T value, T minVal, T maxVal) {
    return value >= minVal && value <= maxVal;
}

bool ParamManager::hasTag(ParamIndex index, uint32_t tag) {
    return (system_params[index].tags & tag) != 0;
}

std::vector<ParamIndex> ParamManager::getParamsByTag(uint32_t tag) {
    std::vector<ParamIndex> results;

    for (uint16_t i = 0; i < PARAM_COUNT; i++) {
        if (hasTag(static_cast<ParamIndex>(i), tag)) {
            results.push_back(static_cast<ParamIndex>(i));
        }
    }

    return results;
}

bool ParamManager::setFloat(ParamIndex index, float value) {
    ConfigParam& param = system_params[index];
    if (param.type != TYPE_FLOAT) return false;
    if (!validateRange(value, param.floatParam.min, param.floatParam.max)) return false;
    param.floatParam.value = value;
    storageManager.scheduleSave(index);
    return true;
}

bool ParamManager::setUInt(ParamIndex index, uint32_t value) {
    ConfigParam& param = system_params[index];

    switch (param.type) {
        case TYPE_UINT8:
            if (!validateRange<uint8_t>(value, param.uint8Param.min, param.uint8Param.max)) return false;
            param.uint8Param.value = value;
            break;

        case TYPE_UINT16:
            if (!validateRange<uint16_t>(value, param.uint16Param.min, param.uint16Param.max)) return false;
            param.uint16Param.value = value;
            break;

        case TYPE_UINT32:
        case TYPE_TIME_SECONDS:
        case TYPE_COLOR_RGB:
            if (!validateRange<uint32_t>(value, param.uint32Param.min, param.uint32Param.max)) return false;
            param.uint32Param.value = value;
            break;

        case TYPE_BITMASK:
            param.bitmaskParam.value = value;
            break;

        default:
            return false;
    }

    storageManager.scheduleSave(index);
    return true;
}

bool ParamManager::setInt(ParamIndex index, int32_t value) {
    ConfigParam& param = system_params[index];

    switch (param.type) {
        case TYPE_INT8:
            if (!validateRange<int8_t>(value, param.int8Param.min, param.int8Param.max)) return false;
            param.int8Param.value = value;
            break;

        case TYPE_INT16:
            if (!validateRange<int16_t>(value, param.int16Param.min, param.int16Param.max)) return false;
            param.int16Param.value = value;
            break;

        case TYPE_INT32:
            if (!validateRange<int32_t>(value, param.int32Param.min, param.int32Param.max)) return false;
            param.int32Param.value = value;
            break;

        default:
            return false;
    }

    storageManager.scheduleSave(index);
    return true;
}

bool ParamManager::setBool(ParamIndex index, bool value) {
    ConfigParam& param = system_params[index];
    if (param.type != TYPE_BOOL) return false;
    param.boolParam.value = value;
    storageManager.scheduleSave(index);
    return true;
}

bool ParamManager::setString(ParamIndex index, const char* value) {
    ConfigParam& param = system_params[index];

    if (!(param.type == TYPE_STRING ||
          param.type == TYPE_PASSWORD ||
          param.type == TYPE_IP_ADDRESS)) return false;

    if (strlen(value) >= param.stringParam.maxLength) return false;

    strncpy(param.stringParam.value, value, param.stringParam.maxLength - 1);
    param.stringParam.value[param.stringParam.maxLength - 1] = '\0';

    storageManager.scheduleSave(index);
    return true;
}

bool ParamManager::setEnum(ParamIndex index, uint8_t value) {
    ConfigParam& param = system_params[index];
    if (param.type != TYPE_ENUM) return false;
    if (value >= param.enumParam.optionCount) return false;

    param.enumParam.value = value;
    storageManager.scheduleSave(index);
    return true;
}

bool ParamManager::getBool(ParamIndex index) {
    ConfigParam& param = system_params[index];

    if (param.type != TYPE_BOOL) return false;

    return param.boolParam.value;
}

const char* ParamManager::getString(ParamIndex index) {
    ConfigParam& param = system_params[index];

    if (param.type != TYPE_STRING &&
        param.type != TYPE_PASSWORD) {
        return "";
    }

    return param.stringParam.value;
}

void ParamManager::resetParam(ParamIndex index) {
    ConfigParam& param = system_params[index];

    switch (param.type) {

        case TYPE_FLOAT:
            param.floatParam.value = param.floatParam.defaultValue;
            break;

        case TYPE_DOUBLE:
            param.doubleParam.value = param.doubleParam.defaultValue;
            break;

        case TYPE_UINT8:
            param.uint8Param.value = param.uint8Param.defaultValue;
            break;

        case TYPE_UINT16:
            param.uint16Param.value = param.uint16Param.defaultValue;
            break;

        case TYPE_UINT32:
            param.uint32Param.value = param.uint32Param.defaultValue;
            break;

        case TYPE_INT8:
            param.int8Param.value = param.int8Param.defaultValue;
            break;

        case TYPE_INT16:
            param.int16Param.value = param.int16Param.defaultValue;
            break;

        case TYPE_INT32:
            param.int32Param.value = param.int32Param.defaultValue;
            break;

        case TYPE_BOOL:
            param.boolParam.value = param.boolParam.defaultValue;
            break;

        case TYPE_STRING:
        case TYPE_PASSWORD:
            strncpy(
                param.stringParam.value,
                param.stringParam.defaultValue,
                param.stringParam.maxLength
            );
            param.stringParam.value[param.stringParam.maxLength - 1] = '\0';
            break;

        case TYPE_ENUM:
            param.enumParam.value = param.enumParam.defaultValue;
            break;

        case TYPE_BITMASK:
            param.bitmaskParam.value = param.bitmaskParam.defaultValue;
            break;

        default:
            break;
    }

    storageManager.scheduleSave(index);
}

uint32_t ParamManager::getUInt(ParamIndex index) {
    ConfigParam& param = system_params[index];

    switch (param.type) {
        case TYPE_UINT8:
            return param.uint8Param.value;

        case TYPE_UINT16:
            return param.uint16Param.value;

        case TYPE_UINT32:
            return param.uint32Param.value;

        case TYPE_ENUM:
            return param.enumParam.value;

        case TYPE_BITMASK:
            return param.bitmaskParam.value;

        default:
            return 0;
    }
}

int32_t ParamManager::getInt(ParamIndex index) {
    ConfigParam& param = system_params[index];

    switch (param.type) {

        case TYPE_INT8:
            return param.int8Param.value;

        case TYPE_INT16:
            return param.int16Param.value;

        case TYPE_INT32:
            return param.int32Param.value;

        default:
            return 0;
    }
}