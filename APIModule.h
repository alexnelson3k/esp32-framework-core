// APIModule.h

#ifndef API_MODULE_H
#define API_MODULE_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_https_server.h>
#include <cJSON.h>

#include "BaseModule.h"
#include "ParamManager.h"
#include "ParamStorageManager.h"
#include "Param_Types.h"

class APIModule : public BaseModule {
private:
    ParamManager& paramManager;
    ParamStorageManager& storageManager;

    httpd_handle_t server;
    bool running;

    httpd_uri_t config_uri;
    httpd_uri_t set_uri;
    httpd_uri_t status_uri;

    bool startServer();
    void stopServer();

    bool isAuthorized(httpd_req_t* req);
    void sendUnauthorized(httpd_req_t* req);

    esp_err_t processConfig(httpd_req_t* req);
    esp_err_t processSet(httpd_req_t* req);
    esp_err_t processStatus(httpd_req_t* req);

    bool setParamFromJSON(ParamIndex index, cJSON* item);

    static esp_err_t configHandler(httpd_req_t* req);
    static esp_err_t setHandler(httpd_req_t* req);
    static esp_err_t statusHandler(httpd_req_t* req);

    esp_err_t processHealth(httpd_req_t* req);
    static esp_err_t healthHandler(httpd_req_t* req);
    httpd_uri_t healthUri;

public:
    APIModule(ParamManager& pm, ParamStorageManager& sm);

    bool isRunning() const { return running; }

    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;
};

#endif