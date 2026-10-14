// Compile/run with a C++17 host compiler; no Arduino dependencies are needed.
#define LOGGER_MODULE_H
enum LogLevel { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG };
class LoggerModule {
public:
    int limit = LOG_INFO;
    int calls = 0;
    bool shouldLog(LogLevel level) const { return level <= limit; }
    template <typename... Args> void logf(LogLevel, const char*, const char*, Args...) { ++calls; }
};
LoggerModule* globalLogger = nullptr;
unsigned long testTime = 0;
unsigned long millis() { return testTime; }
#include "../src/FrameworkLog.h"

void rateLimited(int& argumentEvaluations) {
    FW_LOG_EVERY(LOG_WARN, 1000, "Test", "%d", ++argumentEvaluations);
}
int main() {
    static LoggerModule sink;
    globalLogger = &sink;
    int arguments = 0;
    FW_LOG_DEBUG("Test", "%d", ++arguments);
    if (arguments != 0 || sink.calls != 0) return 1;
    FW_LOG_ERROR("Test", "%d", ++arguments);
    if (arguments != 1 || sink.calls != 1) return 2;
    rateLimited(arguments);
    rateLimited(arguments);
    if (arguments != 2 || sink.calls != 2) return 3;
    testTime = 999;
    rateLimited(arguments);
    if (sink.calls != 2) return 4;
    testTime = 1000;
    rateLimited(arguments);
    if (arguments != 3 || sink.calls != 3) return 5;
    sink.limit = LOG_DEBUG;
    FW_LOG_DEBUG("Test", "Enabled");
    if (sink.calls != 4) return 6;
    globalLogger = nullptr;
    FW_LOG_ERROR("Test", "%d", ++arguments);
    return arguments == 3 ? 0 : 7;
}
