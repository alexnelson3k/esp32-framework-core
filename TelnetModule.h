// TelnetModule.h

#ifndef TELNET_MODULE_H
#define TELNET_MODULE_H

#include <Arduino.h>
#include <WiFi.h>
#include "BaseModule.h"
#include "ParamManager.h"
#include "CommandProcessor.h"

class TelnetModule : public BaseModule {
public:
    TelnetModule(ParamManager& pm, CommandProcessor& cp);

    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;
    bool hasClient() const;

private:
    ParamManager& paramManager;
    CommandProcessor& commandProcessor;

    WiFiServer* telnetServer;
    WiFiClient telnetClient;

    bool serverRunning;
    uint16_t currentPort;

    void startServer();
    void stopServer();
    void restartServerIfNeeded();
    bool wifiAvailable();
    
    static const uint16_t DEFAULT_PORT = 23;
};

#endif