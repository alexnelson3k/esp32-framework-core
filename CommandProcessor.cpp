#include "CommandProcessor.h"

#include <WiFi.h>

#include "TimeManager.h"
#include "LoggerModule.h"
#include "WiFiManagerModule.h"
#include "MQTTModule.h"
#include "TelnetModule.h"
#include "APIModule.h"
#include "WatchdogModule.h"
#include "Param_Types.h"
#include "ParamStorageManager.h"

extern TimeManager timeManager;
extern LoggerModule loggerModule;
extern WiFiManagerModule wifiManager;
extern MQTTModule mqttModule;
extern TelnetModule telnetModule;
extern APIModule apiModule;
extern WatchdogModule watchdogModule;

CommandProcessor::CommandProcessor(ParamManager& pm, ParamStorageManager& sm)
    : BaseModule("CommandProcessor"),
      paramManager(pm),
      storageManager(sm) {}

bool CommandProcessor::init() {
    initialized = true;
    return true;
}

void CommandProcessor::start() {
    enabled = true;
}

void CommandProcessor::stop() {
    enabled = false;
}

void CommandProcessor::loop() {
    if (enabled) {
        handleSerialCommands();
    }
}

void CommandProcessor::handleSerialCommands() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        handleCommand(input, Serial);
    }
}

void CommandProcessor::handleCommand(String input, Stream& output) {
    input.trim();
    if (input.length() == 0) return;

    String cmd, arg1, arg2;

    int firstSpace = input.indexOf(' ');
    if (firstSpace == -1) {
        cmd = input;
    } else {
        cmd = input.substring(0, firstSpace);

        int secondSpace = input.indexOf(' ', firstSpace + 1);
        if (secondSpace == -1) {
            arg1 = input.substring(firstSpace + 1);
        } else {
            arg1 = input.substring(firstSpace + 1, secondSpace);
            arg2 = input.substring(secondSpace + 1);
        }
    }

    cmd.toLowerCase();

    if (processCoreCommand(cmd, arg1, arg2, output)) return;
    if (processParamCommand(cmd, arg1, arg2, output)) return;

    output.println("Error: Unknown command. Type 'help' for available commands.");
}

bool CommandProcessor::processCoreCommand(const String& cmd, const String&, const String&, Stream& output) {
    if (cmd == "help") {
        showHelp(output);
        return true;
    }

    if (cmd == "status") {
    printExtendedStatus(output);
    return true;
    }

    if (cmd == "list") {
        showAllParameters(output);
        return true;
    }

    if (cmd == "save") {
        storageManager.saveAll();
        output.println("Configuration saved.");
        return true;
    }

    if (cmd == "factory_reset") {
        storageManager.factoryReset();
        storageManager.saveAll();
        output.println("Factory defaults restored.");
        return true;
    }

    if (cmd == "reboot") {
        output.println("Rebooting...");
        delay(1000);
        ESP.restart();
        return true;
    }

    if (cmd == "clear") {
        output.print("\033[2J\033[H");
        return true;
    }

    return false;
}

bool CommandProcessor::processParamCommand(const String& cmd, const String& paramName, const String& value, Stream& output) {
    if (paramName.length() == 0) return false;

    ParamIndex index = findParamByName(paramName);
    if (index == PARAM_COUNT) {
        output.println("Error: Parameter not found.");
        return true;
    }

    switch (cmd[0]) {
        case 'g':
            if (cmd == "get") return handleGet(index, output);
            break;

        case 's':
            if (cmd == "set") return handleSet(index, value, output);
            break;

        case 'i':
            if (cmd == "info") return handleInfo(index, output);
            break;

        case 'r':
            if (cmd == "reset") return handleReset(index, output);
            break;
    }

    return false;
}

ParamIndex CommandProcessor::findParamByName(const String& name) {
    for (uint16_t i = 0; i < PARAM_COUNT; i++) {
        if (name.equalsIgnoreCase(system_params[i].name)) {
            return static_cast<ParamIndex>(i);
        }
    }
    return PARAM_COUNT;
}

bool CommandProcessor::isParamAccessible(const ConfigParam& param) {
    return (param.flags & FLAG_SERIAL_ACCESS);
}

bool CommandProcessor::handleGet(ParamIndex index, Stream& output) {
    ConfigParam& param = system_params[index];

    if (!isParamAccessible(param)) {
        output.println("Error: Access denied.");
        return true;
    }

    output.print(param.name);
    output.print(" = ");
    output.println(maskIfSecured(param));

    return true;
}

bool CommandProcessor::handleSet(ParamIndex index, const String& value, Stream& output) {
    ConfigParam& param = system_params[index];

    if (!isParamAccessible(param)) {
        output.println("Error: Access denied.");
        return true;
    }

    if (param.flags & FLAG_READ_ONLY) {
        output.println("Error: Parameter is read-only.");
        return true;
    }

    bool success = false;

    switch (param.type) {
        case TYPE_FLOAT:
            success = paramManager.setFloat(index, value.toFloat());
            break;

        case TYPE_UINT8:
        case TYPE_UINT16:
        case TYPE_UINT32:
            success = paramManager.setUInt(index, value.toInt());
            break;

        case TYPE_INT8:
        case TYPE_INT16:
        case TYPE_INT32:
            success = paramManager.setInt(index, value.toInt());
            break;

        case TYPE_BOOL:
            success = paramManager.setBool(
                index,
                value.equalsIgnoreCase("true") ||
                value == "1" ||
                value.equalsIgnoreCase("on")
            );
            break;

        case TYPE_STRING:
        case TYPE_PASSWORD:
            success = paramManager.setString(index, value.c_str());
            break;

        default:
            output.println("Error: Unsupported parameter type.");
            return true;
    }

    if (success) {
        output.println("Parameter updated successfully.");
    } else {
        output.println("Error: Invalid value or range.");
    }

    return true;
}

bool CommandProcessor::handleInfo(ParamIndex index, Stream& output) {
    ConfigParam& param = system_params[index];

    output.println("=== PARAM INFO ===");
    output.println("Name: " + String(param.name));
    output.println("Description: " + String(param.description));
    output.println("Type: " + typeToString(param.type));

    if (param.flags & FLAG_READ_ONLY) output.println("Read Only: Yes");
    if (param.flags & FLAG_SECURED) output.println("Secured: Yes");
    if (param.flags & FLAG_REQUIRES_REBOOT) output.println("Requires Reboot: Yes");

    return true;
}

bool CommandProcessor::handleReset(ParamIndex index, Stream& output) {
    paramManager.resetParam(index);
    output.println("Parameter reset to default.");
    return true;
}

String CommandProcessor::maskIfSecured(const ConfigParam& param) {
    if (param.flags & FLAG_SECURED) {
        return "******";
    }

    return getParamDisplayValue(findParamByName(param.name));
}

void CommandProcessor::showHelp(Stream& output) {
    output.println("=== AVAILABLE COMMANDS ===");
    output.println("help                - Show help");
    output.println("status              - Show system status");
    output.println("list                - Show all parameters");
    output.println("save                - Save configuration");
    output.println("factory_reset       - Restore defaults");
    output.println("reboot              - Restart system");
    output.println("clear               - Clear terminal");
    output.println("");
    output.println("get <param>         - Read parameter");
    output.println("set <param> <value> - Set parameter");
    output.println("info <param>        - Parameter details");
    output.println("reset <param>       - Reset to default");
}

void CommandProcessor::showStatus(Stream& output) {
    output.println("=== SYSTEM STATUS ===");
    output.println("Uptime: " + String(millis() / 1000) + " seconds");
    output.println("Free Heap: " + String(ESP.getFreeHeap()));
}

void CommandProcessor::showAllParameters(Stream& output) {
    output.println("=== PARAMETERS ===");

    for (uint16_t i = 0; i < PARAM_COUNT; i++) {
        ConfigParam& param = system_params[i];

        if (!isParamAccessible(param)) continue;

        output.print(param.name);
        output.print(" = ");
        output.println(maskIfSecured(param));
    }
}

String CommandProcessor::typeToString(ParamType type) {
    switch (type) {
        case TYPE_FLOAT: return "float";
        case TYPE_DOUBLE: return "double";
        case TYPE_UINT8: return "uint8";
        case TYPE_UINT16: return "uint16";
        case TYPE_UINT32: return "uint32";
        case TYPE_INT8: return "int8";
        case TYPE_INT16: return "int16";
        case TYPE_INT32: return "int32";
        case TYPE_BOOL: return "bool";
        case TYPE_STRING: return "string";
        case TYPE_ENUM: return "enum";
        case TYPE_BITMASK: return "bitmask";
        case TYPE_IP_ADDRESS: return "ip";
        case TYPE_PASSWORD: return "password";
        case TYPE_COLOR_RGB: return "rgb";
        case TYPE_TIME_SECONDS: return "time";
        default: return "unknown";
    }
}

void CommandProcessor::printExtendedStatus(Stream& output) {

    output.println("========== SYSTEM STATUS ==========");

    output.print("Device Name: ");
    output.println(paramManager.getString(PARAM_DEVICE_NAME));

    output.print("Schema: ");
    output.println(PARAM_SCHEMA_ID);

    output.print("Registry Version: ");
    output.println(PARAM_REGISTRY_VERSION);

    output.print("Registry Checksum: 0x");
    output.println(storageManager.calculateRegistryChecksum(), HEX);

    output.print("Param Count: ");
    output.println(PARAM_COUNT);

    output.println();

    output.print("Uptime: ");
    output.print(millis() / 1000);
    output.println(" sec");

    output.print("Free Heap: ");
    output.println(ESP.getFreeHeap());

    output.print("Min Free Heap: ");
    output.println(ESP.getMinFreeHeap());

    output.println();

    output.print("WiFi: ");
    output.println(WiFi.isConnected() ? "Connected" : "Disconnected");

    output.print("IP: ");
    output.println(WiFi.localIP().toString());

    output.print("RSSI: ");
    output.print(WiFi.RSSI());
    output.println(" dBm");

    output.println();

    output.print("MQTT: ");
    output.println(mqttModule.isConnected() ? "Connected" : "Disconnected");

    output.print("API: ");
    output.println(apiModule.isRunning() ? "Running" : "Stopped");

    output.print("Telnet: ");
    output.println(telnetModule.hasClient() ? "Connected" : "Idle");

    output.println();

    output.print("Time: ");
    output.println(timeManager.getFormattedTime());

    output.print("Timezone offset: ");
    output.println(paramManager.getInt(PARAM_TIMEZONE_OFFSET_MINUTES));
    

    output.print("NTP Sync: ");
    output.println(timeManager.isTimeValid() ? "OK" : "NOT SYNCED");

    output.println();

    output.print("Watchdog Failures: ");
    output.println(watchdogModule.getFailureCount());

    output.println();

    output.print("Profiling: ");
    output.println(
        paramManager.getBool(PARAM_PROFILING_ENABLED)
        ? "ENABLED"
        : "DISABLED"
    );

    output.println("===================================");
}