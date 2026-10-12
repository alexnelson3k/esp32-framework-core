#include "src/ESP32Framework.h"

#if __has_include("secrets.h")
#include "secrets.h"
#endif
#if __has_include("cert.h")
#include "cert.h"
#define FRAMEWORK_HAS_CERTIFICATE
#endif

static void stringDefault(ParamIndex index, const char* value) {
    ConfigParam& param = system_params[index];
    param.stringParam.defaultValue = value;
    strncpy(param.stringParam.value, value, param.stringParam.maxLength - 1);
    param.stringParam.value[param.stringParam.maxLength - 1] = '\0';
}

void setup() {
#ifdef DEFAULT_SSID
    stringDefault(PARAM_WIFI_SSID, DEFAULT_SSID);
#endif
#ifdef DEFAULT_WIFI_PASSWORD
    stringDefault(PARAM_WIFI_PASSWORD, DEFAULT_WIFI_PASSWORD);
#endif
#ifdef DEFAULT_MQTT_SERVER
    stringDefault(PARAM_MQTT_SERVER, DEFAULT_MQTT_SERVER);
#endif
#ifdef DEFAULT_MQTT_USERNAME
    stringDefault(PARAM_MQTT_USERNAME, DEFAULT_MQTT_USERNAME);
#endif
#ifdef DEFAULT_MQTT_PASSWORD
    stringDefault(PARAM_MQTT_PASSWORD, DEFAULT_MQTT_PASSWORD);
#endif
#ifdef DEFAULT_API_TOKEN
    stringDefault(PARAM_API_TOKEN, DEFAULT_API_TOKEN);
#endif
#ifdef FRAMEWORK_HAS_CERTIFICATE
    frameworkConfigureTLS(server_cert, server_key);
#else
    system_params[PARAM_API_ENABLED].boolParam.value = false;
    system_params[PARAM_API_ENABLED].boolParam.defaultValue = false;
#endif
    frameworkBegin();
}

void loop() {
    frameworkLoop();
}
