// TelnetModule.cpp

#include "TelnetModule.h"

TelnetModule::TelnetModule(ParamManager& pm, CommandProcessor& cp)
    : BaseModule("Telnet"),
      paramManager(pm),
      commandProcessor(cp),
      telnetServer(nullptr),
      serverRunning(false),
      currentPort(DEFAULT_PORT) {}

bool TelnetModule::init() {
    initialized = true;
    return true;
}

void TelnetModule::start() {
    if (paramManager.getBool(PARAM_TELNET_ENABLED)) {
        startServer();
    }
}

void TelnetModule::stop() {
    stopServer();
}

bool TelnetModule::wifiAvailable() {
    return WiFi.status() == WL_CONNECTED || WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA;
}

void TelnetModule::startServer() {
    if (serverRunning) return;
    if (!wifiAvailable()) return;

    currentPort = paramManager.getUInt(PARAM_TELNET_PORT);
    if (currentPort == 0) currentPort = DEFAULT_PORT;

    telnetServer = new WiFiServer(currentPort);
    telnetServer->begin();
    telnetServer->setNoDelay(true);

    serverRunning = true;
}

void TelnetModule::stopServer() {
    if (!serverRunning) return;

    if (telnetClient && telnetClient.connected()) {
        telnetClient.println("Telnet server shutting down...");
        telnetClient.stop();
    }

    if (telnetServer) {
        telnetServer->stop();
        delete telnetServer;
        telnetServer = nullptr;
    }

    serverRunning = false;
}

void TelnetModule::restartServerIfNeeded() {
    uint16_t desiredPort = paramManager.getUInt(PARAM_TELNET_PORT);
    if (desiredPort == 0) desiredPort = DEFAULT_PORT;

    if (desiredPort != currentPort) {
        stopServer();
        startServer();
    }
}

void TelnetModule::loop() {
    bool enabled = paramManager.getBool(PARAM_TELNET_ENABLED);

    if (!enabled) {
        if (serverRunning) stopServer();
        return;
    }

    if (!wifiAvailable()) {
        if (serverRunning) stopServer();
        return;
    }

    if (!serverRunning) {
        startServer();
        return;
    }

    restartServerIfNeeded();

    if (telnetServer->hasClient()) {
        if (!telnetClient || !telnetClient.connected()) {
            telnetClient = telnetServer->available();
            telnetClient.println("ESP32 Telnet Console Ready");
            telnetClient.print("> ");
        } else {
            WiFiClient newClient = telnetServer->available();
            newClient.println("Another client is already connected.");
            newClient.stop();
        }
    }

    if (telnetClient && telnetClient.connected() && telnetClient.available()) {
        String input = telnetClient.readStringUntil('\n');
        input.trim();

        if (input.length() > 0) {
            commandProcessor.handleCommand(input, telnetClient);
        }

        telnetClient.print("> ");
    }

    if (telnetClient && !telnetClient.connected()) {
        telnetClient.stop();
    }
}

bool TelnetModule::hasClient() const {
    return telnetClient && telnetClient.connected();
}