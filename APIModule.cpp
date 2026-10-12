// APIModule.cpp

#include "APIModule.h"
#include "cert.h"

extern ConfigParam system_params[PARAM_COUNT];

APIModule::APIModule(ParamManager& pm, ParamStorageManager& sm)
    : BaseModule("API"),
      paramManager(pm),
      storageManager(sm),
      server(nullptr),
      running(false) {

    memset(&config_uri, 0, sizeof(config_uri));
    config_uri.uri = "/api/config";
    config_uri.method = HTTP_GET;
    config_uri.handler = configHandler;
    config_uri.user_ctx = this;

    memset(&set_uri, 0, sizeof(set_uri));
    set_uri.uri = "/api/set";
    set_uri.method = HTTP_POST;
    set_uri.handler = setHandler;
    set_uri.user_ctx = this;

    memset(&status_uri, 0, sizeof(status_uri));
    status_uri.uri = "/api/status";
    status_uri.method = HTTP_GET;
    status_uri.handler = statusHandler;
    status_uri.user_ctx = this;

    memset(&healthUri, 0, sizeof(healthUri));
    healthUri.uri = "/api/health";
    healthUri.method = HTTP_GET;
    healthUri.handler = healthHandler;
    healthUri.user_ctx = this;
}

bool APIModule::init() {
    initialized = true;
    return true;
}

void APIModule::start() {
    if (paramManager.getBool(PARAM_API_ENABLED)) {
        running = startServer();
        enabled = running;
    }
}

void APIModule::stop() {
    stopServer();
    running = false;
    enabled = false;
}

void APIModule::loop() {
    bool shouldRun = paramManager.getBool(PARAM_API_ENABLED);

    if (shouldRun && !running) {
        start();
    } else if (!shouldRun && running) {
        stop();
    }
}

bool APIModule::startServer() {
    httpd_ssl_config_t conf = HTTPD_SSL_CONFIG_DEFAULT();

    conf.httpd.server_port = paramManager.getUInt(PARAM_API_PORT);

    conf.servercert = (const uint8_t*)server_cert;
    conf.servercert_len = strlen(server_cert) + 1;

    conf.prvtkey_pem = (const uint8_t*)server_key;
    conf.prvtkey_len = strlen(server_key) + 1;

    conf.httpd.max_open_sockets = 1;
    conf.httpd.recv_wait_timeout = 10;
    conf.httpd.send_wait_timeout = 10;
    conf.httpd.lru_purge_enable = true;

    if (httpd_ssl_start(&server, &conf) != ESP_OK) {
        return false;
    }

    httpd_register_uri_handler(server, &config_uri);
    httpd_register_uri_handler(server, &set_uri);
    httpd_register_uri_handler(server, &status_uri);
    httpd_register_uri_handler(server, &healthUri);

    return true;
}

void APIModule::stopServer() {
    if (server) {
        httpd_ssl_stop(server);
        server = nullptr;
    }
}

bool APIModule::isAuthorized(httpd_req_t* req) {
    char auth[256];

    if (httpd_req_get_hdr_value_str(req, "Authorization", auth, sizeof(auth)) != ESP_OK) {
        return false;
    }

    String expected = "Bearer ";
    expected += paramManager.getString(PARAM_API_TOKEN);

    return String(auth) == expected;
}

void APIModule::sendUnauthorized(httpd_req_t* req) {
    httpd_resp_set_status(req, "401 Unauthorized");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, "{\"error\":\"Unauthorized\"}");
}

// ============================================================
// GET /api/config
// ============================================================

esp_err_t APIModule::processConfig(httpd_req_t* req) {
    if (!isAuthorized(req)) {
        sendUnauthorized(req);
        return ESP_OK;
    }

    cJSON* root = cJSON_CreateObject();

    for (int i = 0; i < PARAM_COUNT; i++) {
        ConfigParam& p = system_params[i];

        if (!(p.flags & FLAG_API_ACCESS)) continue;

        if (p.flags & FLAG_SECURED) {
            cJSON_AddStringToObject(root, p.name, "******");
            continue;
        }

        switch (p.type) {

            case TYPE_BOOL:
                cJSON_AddBoolToObject(root, p.name, p.boolParam.value);
                break;

            case TYPE_UINT8:
                cJSON_AddNumberToObject(root, p.name, p.uint8Param.value);
                break;

            case TYPE_UINT16:
                cJSON_AddNumberToObject(root, p.name, p.uint16Param.value);
                break;

            case TYPE_UINT32:
                cJSON_AddNumberToObject(root, p.name, p.uint32Param.value);
                break;

            case TYPE_INT8:
                cJSON_AddNumberToObject(root, p.name, p.int8Param.value);
                break;

            case TYPE_INT16:
                cJSON_AddNumberToObject(root, p.name, p.int16Param.value);
                break;

            case TYPE_INT32:
                cJSON_AddNumberToObject(root, p.name, p.int32Param.value);
                break;

            case TYPE_FLOAT:
                cJSON_AddNumberToObject(root, p.name, p.floatParam.value);
                break;

            case TYPE_DOUBLE:
                cJSON_AddNumberToObject(root, p.name, p.doubleParam.value);
                break;

            case TYPE_STRING:
            case TYPE_PASSWORD:
                cJSON_AddStringToObject(root, p.name, p.stringParam.value);
                break;

            case TYPE_ENUM:
                cJSON_AddNumberToObject(root, p.name, p.enumParam.value);
                break;

            default:
                break;
        }
    }

    char* response = cJSON_PrintUnformatted(root);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, response);

    free(response);
    cJSON_Delete(root);

    return ESP_OK;
}

// ============================================================
// POST /api/set
// ============================================================

esp_err_t APIModule::processSet(httpd_req_t* req) {
    if (!isAuthorized(req)) {
        sendUnauthorized(req);
        return ESP_OK;
    }

    if (req->content_len > 2048) {
        httpd_resp_send_err(req, HTTPD_413_CONTENT_TOO_LARGE, "Payload too large");
        return ESP_OK;
    }

    char buffer[2049];
    int received = httpd_req_recv(req, buffer, req->content_len);

    if (received <= 0) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
        return ESP_OK;
    }

    buffer[received] = '\0';

    cJSON* root = cJSON_Parse(buffer);

    if (!root) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
        return ESP_OK;
    }

    int success = 0;
    int failed = 0;

    cJSON* item = nullptr;

    cJSON_ArrayForEach(item, root) {

        ParamIndex foundIndex = PARAM_COUNT;

        for (int i = 0; i < PARAM_COUNT; i++) {
            if (strcmp(system_params[i].name, item->string) == 0) {
                foundIndex = (ParamIndex)i;
                break;
            }
        }

        if (foundIndex == PARAM_COUNT) {
            failed++;
            continue;
        }

        ConfigParam& param = system_params[foundIndex];

        if (!(param.flags & FLAG_API_ACCESS)) {
            failed++;
            continue;
        }

        if (setParamFromJSON(foundIndex, item)) {
            storageManager.scheduleSave(foundIndex);
            success++;
        } else {
            failed++;
        }
    }

    cJSON_Delete(root);

    cJSON* response = cJSON_CreateObject();
    cJSON_AddNumberToObject(response, "success", success);
    cJSON_AddNumberToObject(response, "failed", failed);

    char* output = cJSON_PrintUnformatted(response);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, output);

    free(output);
    cJSON_Delete(response);

    return ESP_OK;
}

// ============================================================
// GET /api/status
// ============================================================

esp_err_t APIModule::processStatus(httpd_req_t* req) {
    if (!isAuthorized(req)) {
        sendUnauthorized(req);
        return ESP_OK;
    }

    cJSON* root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "device", paramManager.getString(PARAM_DEVICE_NAME));
    cJSON_AddStringToObject(root, "ip", WiFi.localIP().toString().c_str());
    cJSON_AddStringToObject(root, "ssid", WiFi.SSID().c_str());
    cJSON_AddBoolToObject(root, "wifi_connected", WiFi.status() == WL_CONNECTED);
    cJSON_AddNumberToObject(root, "rssi", WiFi.RSSI());
    cJSON_AddNumberToObject(root, "free_heap", ESP.getFreeHeap());

    char* response = cJSON_PrintUnformatted(root);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, response);

    free(response);
    cJSON_Delete(root);

    return ESP_OK;
}

// ============================================================
// Add new endpoint:
// GET /api/health
// ============================================================

esp_err_t APIModule::processHealth(httpd_req_t* req) {
    if (!isAuthorized(req)) {
        sendUnauthorized(req);
        return ESP_OK;
    }

    cJSON* root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "uptime_sec", millis() / 1000);
    cJSON_AddNumberToObject(root, "free_heap", ESP.getFreeHeap());
    cJSON_AddNumberToObject(root, "min_free_heap", ESP.getMinFreeHeap());
    cJSON_AddNumberToObject(root, "wifi_rssi", WiFi.RSSI());
    cJSON_AddStringToObject(root, "ip", WiFi.localIP().toString().c_str());
    cJSON_AddNumberToObject(root, "reset_reason", esp_reset_reason());

    char* response = cJSON_PrintUnformatted(root);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, response);

    free(response);
    cJSON_Delete(root);

    return ESP_OK;
}

// ============================================================
// Parameter Setter
// ============================================================

bool APIModule::setParamFromJSON(ParamIndex index, cJSON* item) {
    ConfigParam& param = system_params[index];

    switch (param.type) {

        case TYPE_BOOL:
            if (cJSON_IsBool(item)) {
                param.boolParam.value = cJSON_IsTrue(item);
                return true;
            }
            break;

        case TYPE_UINT8:
            if (cJSON_IsNumber(item)) {
                param.uint8Param.value = item->valueint;
                return true;
            }
            break;

        case TYPE_UINT16:
            if (cJSON_IsNumber(item)) {
                param.uint16Param.value = item->valueint;
                return true;
            }
            break;

        case TYPE_UINT32:
            if (cJSON_IsNumber(item)) {
                param.uint32Param.value = item->valueint;
                return true;
            }
            break;

        case TYPE_INT8:
            if (cJSON_IsNumber(item)) {
                param.int8Param.value = item->valueint;
                return true;
            }
            break;

        case TYPE_INT16:
            if (cJSON_IsNumber(item)) {
                param.int16Param.value = item->valueint;
                return true;
            }
            break;

        case TYPE_INT32:
            if (cJSON_IsNumber(item)) {
                param.int32Param.value = item->valueint;
                return true;
            }
            break;

        case TYPE_FLOAT:
            if (cJSON_IsNumber(item)) {
                param.floatParam.value = item->valuedouble;
                return true;
            }
            break;

        case TYPE_DOUBLE:
            if (cJSON_IsNumber(item)) {
                param.doubleParam.value = item->valuedouble;
                return true;
            }
            break;

        case TYPE_STRING:
        case TYPE_PASSWORD:
            if (cJSON_IsString(item)) {
                strncpy(param.stringParam.value,
                        item->valuestring,
                        param.stringParam.maxLength - 1);

                param.stringParam.value[param.stringParam.maxLength - 1] = '\0';
                return true;
            }
            break;

        case TYPE_ENUM:
            if (cJSON_IsNumber(item)) {
                param.enumParam.value = item->valueint;
                return true;
            }
            break;

        default:
            break;
    }

    return false;
}

// ============================================================
// Static URI Wrappers
// ============================================================

esp_err_t APIModule::configHandler(httpd_req_t* req) {
    return ((APIModule*)req->user_ctx)->processConfig(req);
}

esp_err_t APIModule::setHandler(httpd_req_t* req) {
    return ((APIModule*)req->user_ctx)->processSet(req);
}

esp_err_t APIModule::statusHandler(httpd_req_t* req) {
    return ((APIModule*)req->user_ctx)->processStatus(req);
}

esp_err_t APIModule::healthHandler(httpd_req_t* req) {
    return ((APIModule*)req->user_ctx)->processHealth(req);
}
