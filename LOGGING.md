# Runtime Logging

`FrameworkLog.h` is included by `ESP32Framework.h`. Use `FW_LOG_ERROR`, `FW_LOG_WARN`,
`FW_LOG_INFO` and `FW_LOG_DEBUG` with a module name and printf-style message:

```cpp
FW_LOG_INFO("Sensor", "Initialized on GPIO %d", pin);
FW_LOG_ERROR("Sensor", "Reading failed");
FW_LOG_EVERY(LOG_WARN, 30000, "Network", "Connection attempt failed");
```

ERROR identifies failures, WARN degraded operation/retries, INFO lifecycle/state transitions,
and DEBUG detailed diagnostics. `log_level` filters them (ERROR=0, WARN=1, INFO=2, DEBUG=3).
`log_serial_enabled` and `log_syslog_enabled` control the current output channels;
`syslog_min_level` applies an additional syslog verbosity limit. Console command replies
remain direct Stream responses and are not suppressed by logging settings.

Macros check the configured level before evaluating arguments or formatting. Repeated
network failures use rate-limited call sites. Do not call these macros from interrupts,
WiFi/ESP-NOW callbacks, or the logger/syslog send implementation: sinks allocate and use
network services, and logging from the sink can recurse. ESP-NOW failure counters are
reported from the main loop instead. Timestamp formatting does not wait for NTP.

Do not log passwords, tokens, keys, raw request/command payloads or arbitrary parameter
values. Parameter diagnostics report names only. Module lifecycle logging is provided
centrally; getters, control hot paths and interrupt dispatch do not need per-call logs.

Validation: the host macro test passed C++17 syntax/static-analysis checks. Its executable
and the complete ESP32 build were not run in this environment.
