// ============================================================
// ParamStorageManager.cpp (FULL VERSIONING + CHECKSUM SUPPORT)
// ============================================================

#include "ParamStorageManager.h"
#include "Param_Types.h"
#include <CRC32.h>

extern ConfigParam* system_params;

// ============================================================
// Constructor
// ============================================================

ParamStorageManager::ParamStorageManager()
    : lastSaveTime(0) {


}

// ============================================================
// Registry Checksum
// ============================================================

uint32_t ParamStorageManager::calculateRegistryChecksum() {
    CRC32 crc;

    for (uint16_t i = 0; i < system_param_count; i++) {
        ConfigParam& p = system_params[i];

        crc.update((const uint8_t*)p.name, strlen(p.name));
        crc.update((uint8_t*)&p.type, sizeof(p.type));
        crc.update((uint8_t*)&p.flags, sizeof(p.flags));
        crc.update((uint8_t*)&p.tags, sizeof(p.tags));
    }

    return crc.finalize();
}

// ============================================================
// Header Validation
// ============================================================

bool ParamStorageManager::validateStorageHeader() {
    ParamStorageHeader header;

    size_t readSize = preferences.getBytes(
        "header",
        &header,
        sizeof(header)
    );

    if (readSize != sizeof(header)) {
        return false;
    }

    if (header.magic != PARAM_STORAGE_MAGIC) {
        return false;
    }

    if (header.version != PARAM_REGISTRY_VERSION) {
        return false;
    }

    if (header.registryChecksum != calculateRegistryChecksum()) {
        return false;
    }

    return true;
}

// ============================================================
// Save Header
// ============================================================

void ParamStorageManager::saveStorageHeader() {
    ParamStorageHeader header;

    header.magic = PARAM_STORAGE_MAGIC;
    header.version = PARAM_REGISTRY_VERSION;
    header.registryChecksum = calculateRegistryChecksum();

    preferences.putBytes(
        "header",
        &header,
        sizeof(header)
    );
}

// ============================================================
// Factory Reset
// ============================================================

void ParamStorageManager::resetToFactoryDefaults() {
    for (uint16_t i = 0; i < system_param_count; i++) {
        ConfigParam& p = system_params[i];

        switch (p.type) {

            case TYPE_FLOAT:
                p.floatParam.value = p.floatParam.defaultValue;
                break;

            case TYPE_DOUBLE:
                p.doubleParam.value = p.doubleParam.defaultValue;
                break;

            case TYPE_UINT8:
                p.uint8Param.value = p.uint8Param.defaultValue;
                break;

            case TYPE_UINT16:
                p.uint16Param.value = p.uint16Param.defaultValue;
                break;

            case TYPE_UINT32:
            case TYPE_TIME_SECONDS:
            case TYPE_COLOR_RGB:
                p.uint32Param.value = p.uint32Param.defaultValue;
                break;

            case TYPE_INT8:
                p.int8Param.value = p.int8Param.defaultValue;
                break;

            case TYPE_INT16:
                p.int16Param.value = p.int16Param.defaultValue;
                break;

            case TYPE_INT32:
                p.int32Param.value = p.int32Param.defaultValue;
                break;

            case TYPE_BOOL:
                p.boolParam.value = p.boolParam.defaultValue;
                break;

            case TYPE_STRING:
            case TYPE_PASSWORD:
            case TYPE_IP_ADDRESS:
                strncpy(
                    p.stringParam.value,
                    p.stringParam.defaultValue,
                    p.stringParam.maxLength - 1
                );
                p.stringParam.value[p.stringParam.maxLength - 1] = '\0';
                break;

            case TYPE_ENUM:
                p.enumParam.value = p.enumParam.defaultValue;
                break;

            case TYPE_BITMASK:
                p.bitmaskParam.value = p.bitmaskParam.defaultValue;
                break;

            default:
                break;
        }
    }
}

// ============================================================
// Init
// ============================================================

bool ParamStorageManager::init() {
    sealParameterRegistry();
    dirtyFlags.resize(system_param_count, 0);
    if (!preferences.begin("params", false)) {
        return false;
    }

    if (!validateStorageHeader()) {

    Serial.println("[WARNING][STORAGE] Registry mismatch detected.");
    Serial.println("[WARNING][STORAGE] Restoring factory defaults...");
    
    resetToFactoryDefaults();
    saveAll();
    saveStorageHeader();

    Serial.println("[WARNING][STORAGE] Rebooting system...");
    
    delay(2000);
    ESP.restart();
    } else {
        loadAll();
    }

    return true;
}

// ============================================================
// Schedule Save
// ============================================================

void ParamStorageManager::scheduleSave(ParamIndex index) {
    if (index < system_param_count) {
        dirtyFlags[index] = true;
    }
}

// ============================================================
// Save Process
// ============================================================

void ParamStorageManager::process() {
    unsigned long now = millis();

    if (now - lastSaveTime < 1000) {
        return;
    }

    lastSaveTime = now;

    for (uint16_t i = 0; i < system_param_count; i++) {
        if (dirtyFlags[i]) {
            saveParam((ParamIndex)i);
            dirtyFlags[i] = false;
        }
    }
}

// ============================================================
// Save Single Param
// ============================================================

bool ParamStorageManager::saveParam(ParamIndex index) {
    if (index >= system_param_count) return false;

    ConfigParam& p = system_params[index];
    String key = String(index);

    switch (p.type) {

        case TYPE_FLOAT:
            preferences.putFloat(key.c_str(), p.floatParam.value);
            break;

        case TYPE_DOUBLE:
            preferences.putDouble(key.c_str(), p.doubleParam.value);
            break;

        case TYPE_UINT8:
            preferences.putUChar(key.c_str(), p.uint8Param.value);
            break;

        case TYPE_UINT16:
            preferences.putUShort(key.c_str(), p.uint16Param.value);
            break;

        case TYPE_UINT32:
        case TYPE_TIME_SECONDS:
        case TYPE_COLOR_RGB:
            preferences.putUInt(key.c_str(), p.uint32Param.value);
            break;

        case TYPE_INT8:
            preferences.putChar(key.c_str(), p.int8Param.value);
            break;

        case TYPE_INT16:
            preferences.putShort(key.c_str(), p.int16Param.value);
            break;

        case TYPE_INT32:
            preferences.putInt(key.c_str(), p.int32Param.value);
            break;

        case TYPE_BOOL:
            preferences.putBool(key.c_str(), p.boolParam.value);
            break;

        case TYPE_STRING:
        case TYPE_PASSWORD:
        case TYPE_IP_ADDRESS:
            preferences.putString(key.c_str(), p.stringParam.value);
            break;

        case TYPE_ENUM:
            preferences.putUChar(key.c_str(), p.enumParam.value);
            break;

        case TYPE_BITMASK:
            preferences.putUInt(key.c_str(), p.bitmaskParam.value);
            break;

        default:
            return false;
    }

    return true;
}

// ============================================================
// Load Single Param
// ============================================================

bool ParamStorageManager::loadParam(ParamIndex index) {
    if (index >= system_param_count) return false;

    ConfigParam& p = system_params[index];
    String key = String(index);

    switch (p.type) {

        case TYPE_FLOAT:
            p.floatParam.value = preferences.getFloat(key.c_str(), p.floatParam.defaultValue);
            break;

        case TYPE_DOUBLE:
            p.doubleParam.value = preferences.getDouble(key.c_str(), p.doubleParam.defaultValue);
            break;

        case TYPE_UINT8:
            p.uint8Param.value = preferences.getUChar(key.c_str(), p.uint8Param.defaultValue);
            break;

        case TYPE_UINT16:
            p.uint16Param.value = preferences.getUShort(key.c_str(), p.uint16Param.defaultValue);
            break;

        case TYPE_UINT32:
        case TYPE_TIME_SECONDS:
        case TYPE_COLOR_RGB:
            p.uint32Param.value = preferences.getUInt(key.c_str(), p.uint32Param.defaultValue);
            break;

        case TYPE_INT8:
            p.int8Param.value = preferences.getChar(key.c_str(), p.int8Param.defaultValue);
            break;

        case TYPE_INT16:
            p.int16Param.value = preferences.getShort(key.c_str(), p.int16Param.defaultValue);
            break;

        case TYPE_INT32:
            p.int32Param.value = preferences.getInt(key.c_str(), p.int32Param.defaultValue);
            break;

        case TYPE_BOOL:
            p.boolParam.value = preferences.getBool(key.c_str(), p.boolParam.defaultValue);
            break;

        case TYPE_STRING:
        case TYPE_PASSWORD:
        case TYPE_IP_ADDRESS: {
            String val = preferences.getString(
                key.c_str(),
                p.stringParam.defaultValue
            );

            strncpy(
                p.stringParam.value,
                val.c_str(),
                p.stringParam.maxLength - 1
            );

            p.stringParam.value[p.stringParam.maxLength - 1] = '\0';
            break;
        }

        case TYPE_ENUM:
            p.enumParam.value = preferences.getUChar(key.c_str(), p.enumParam.defaultValue);
            break;

        case TYPE_BITMASK:
            p.bitmaskParam.value = preferences.getUInt(key.c_str(), p.bitmaskParam.defaultValue);
            break;

        default:
            return false;
    }

    return true;
}

// ============================================================
// Bulk Save / Load
// ============================================================

void ParamStorageManager::saveAll() {
    for (uint16_t i = 0; i < system_param_count; i++) {
        saveParam((ParamIndex)i);
    }

    saveStorageHeader();
}

void ParamStorageManager::loadAll() {
    for (uint16_t i = 0; i < system_param_count; i++) {
        loadParam((ParamIndex)i);
    }
}

// ============================================================
// Manual Factory Reset
// ============================================================

void ParamStorageManager::factoryReset() {
    preferences.clear();

    resetToFactoryDefaults();

    saveAll();
    saveStorageHeader();
}
