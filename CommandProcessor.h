#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <Arduino.h>
#include "Param_Types.h"
#include "ParamManager.h"
#include "ParamStorageManager.h"
#include "BaseModule.h"

// Forward declarations
class TimeManager;
class LoggerModule;

class CommandProcessor : public BaseModule {
public:
    CommandProcessor(ParamManager& pm, ParamStorageManager& sm);

    // BaseModule required
    bool init() override;
    void start() override;
    void stop() override;
    void loop() override;

    // Command system
    void handleSerialCommands();
    void handleCommand(String input, Stream& output);

    void showHelp(Stream& output);
    void showStatus(Stream& output);
    void showAllParameters(Stream& output);

private:
    ParamManager& paramManager;
    ParamStorageManager& storageManager;

    bool processCoreCommand(
        const String& cmd,
        const String& arg1,
        const String& arg2,
        Stream& output
    );

    bool processParamCommand(
        const String& cmd,
        const String& paramName,
        const String& value,
        Stream& output
    );

    ParamIndex findParamByName(const String& name);
    bool isParamAccessible(const ConfigParam& param);

    bool handleGet(ParamIndex index, Stream& output);
    bool handleSet(ParamIndex index, const String& value, Stream& output);
    bool handleInfo(ParamIndex index, Stream& output);
    bool handleReset(ParamIndex index, Stream& output);

    String maskIfSecured(const ConfigParam& param);
    String typeToString(ParamType type);

    void printExtendedStatus(Stream& output);
};

#endif