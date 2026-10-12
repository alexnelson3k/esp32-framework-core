#ifndef SYSLOG_MODULE_H
#define SYSLOG_MODULE_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "BaseModule.h"
#include "LoggerModule.h"
#include "ParamManager.h"

class SyslogModule : public BaseModule {
private:
    ParamManager& paramManager;
    WiFiUDP udp;

    uint8_t mapSeverity(LogLevel level);
    String buildMessage(LogLevel level, const char* module, const String& message);

public:
    explicit SyslogModule(ParamManager& pm);

    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;

    bool isReady();
    void send(LogLevel level, const char* module, const String& message);
};

extern SyslogModule* globalSyslog;

#endif
