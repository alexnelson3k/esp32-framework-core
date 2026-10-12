# Arduino Library and Child Projects

The common code lives in src and is packaged as ESP32 Framework Core 0.2.0. A child includes ESP32Framework.h and owns its hardware, credentials, parameter definitions and module objects.

```cpp
#include <ESP32Framework.h>

constexpr ParamIndex PARAM_PROJECT_VALUE = PARAM_COUNT;
ConfigParam projectParameters[] = {
    floatParam("project_value").defaultValue(20.0f)
        .range(0.0f, 100.0f).step(0.5f)
        .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
        .tags(TAG_CONTROL).build()
};

void setup() {
    if (!registerProjectParameters(projectParameters, 1)) return;
    // Register application BaseModule instances before frameworkBegin().
    frameworkBegin();
}
void loop() { frameworkLoop(); }
```

PARAM_COUNT is the built-in parameter count. system_param_count is the combined runtime count. ParamIndex is a uint16_t ID; application indexes begin at PARAM_COUNT. Registration copies records, retains their string/enum pointers, and rejects duplicate names. Supply string literals or static arrays. Register blocks in a deterministic order and do not retain registry pointers before registration completes. Storage initialization seals the registry; later registration is rejected.

The core sketch FrameWork.ino remains usable from the repository checkout. It reads optional local secrets.h and cert.h. A library build never includes application credentials. A project can configure TLS with frameworkConfigureTLS(server_cert, server_key) before frameworkBegin(). Core defaults can be overridden before initialization; stored values take precedence on subsequent boots.

Registry checksum changes deliberately initialize defaults using the existing storage policy. Child projects use distinct device flash; their old EEPROM data is not imported. Record the tested library commit with each child release. Update the shared library, rebuild and upload child firmware to apply core fixes.

Disabled WiFi/MQTT/watchdog modules are omitted from startup registration because their existing init methods return false when disabled. Enable those modules and reboot to register them. Other core module behavior remains as before.

Known limitations: saveAll/loadAll currently do not filter FLAG_SAVE_TO_FLASH; runtime-only values need reboot verification. Core functions use global managers, so one Framework runtime per firmware is supported. Optional module compilation is selected by the library's BuildConfig.h. Packaging does not repair existing actuator/PID logic.
