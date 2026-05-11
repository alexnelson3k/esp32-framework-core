#include "SyslogModule.h"

SyslogModule* globalSyslog = nullptr;

SyslogModule::SyslogModule(ParamManager& pm)
    : BaseModule("Syslog"),
      paramManager(pm) {
    globalSyslog = this;
}

bool SyslogModule::init() {
    initialized = true;
    enabled = paramManager.getBool(PARAM_SYSLOG_ENABLED);
    return true;
}

void SyslogModule::start() {
    enabled = paramManager.getBool(PARAM_SYSLOG_ENABLED);
}

void SyslogModule::stop() {
    enabled = false;
    udp.stop();
}

void SyslogModule::loop() {
    enabled = paramManager.getBool(PARAM_SYSLOG_ENABLED);
}

bool SyslogModule::isReady() {
    return enabled &&
           WiFi.status() == WL_CONNECTED &&
           strlen(paramManager.getString(PARAM_SYSLOG_SERVER)) > 0;
}

uint8_t SyslogModule::mapSeverity(LogLevel level) {
    switch (level) {
        case LOG_ERROR: return 3;
        case LOG_WARN:  return 4;
        case LOG_INFO:  return 6;
        case LOG_DEBUG: return 7;
        default:        return 6;
    }
}

String SyslogModule::buildMessage(
    LogLevel level,
    const char* module,
    const String& message
) {
    uint8_t facility = paramManager.getUInt(PARAM_SYSLOG_FACILITY);
    uint8_t priority = facility * 8 + mapSeverity(level);

    String device = paramManager.getString(PARAM_DEVICE_NAME);

    String packet = "<" + String(priority) + ">";
    packet += device;
    packet += " ";
    packet += module;
    packet += ": ";
    packet += message;

    return packet;
}

void SyslogModule::send(LogLevel level, const char* module, const String& message) {
    if (!isReady()) return;

    uint8_t minLevel = paramManager.getUInt(PARAM_SYSLOG_MIN_LEVEL);
    if (level > minLevel) return;

    String packet = buildMessage(level, module, message);

    udp.beginPacket(
        paramManager.getString(PARAM_SYSLOG_SERVER),
        paramManager.getUInt(PARAM_SYSLOG_PORT)
    );
    udp.write((const uint8_t*)packet.c_str(), packet.length());
    udp.endPacket();
}
