# Parameter Definitions

Include `Param_Builders.h` to describe a parameter with typed functions. Builders return the existing `ConfigParam` structure and do not allocate heap memory.

```cpp
[PARAM_TEMP_SETPOINT] = floatParam("temp_setpoint")
    .description("Temperature Setpoint")
    .defaultValue(22.5f).range(0.0f, 100.0f).step(0.1f)
    .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
    .tags(TAG_CONTROL | TAG_TEMPERATURE).build(),
```

`defaultValue()` sets both the initial value and factory default. Numeric builders are `floatParam`, `doubleParam`, `uint8Param`, `uint16Param`, `uint32Param`, `int8Param`, `int16Param`, and `int32Param`. Use literals appropriate for the selected numeric type; C++ still permits implicit numeric conversions.

```cpp
[PARAM_SENSOR_ENABLED] = boolParam("sensor_enabled")
    .description("Enable Sensors").defaultValue(true)
    .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
    .tags(TAG_SENSOR).build(),

[PARAM_WIFI_PASSWORD] = passwordParam("wifi_password")
    .description("WiFi Password")
    .maxLength(64).defaultValue(DEFAULT_WIFI_PASSWORD)
    .access(FLAG_SERIAL_ACCESS | FLAG_API_ACCESS).persistent()
    .tags(TAG_NETWORK | TAG_SECURITY).build(),
```

`stringParam` and `passwordParam` use the existing 128-byte buffer. `maxLength()` is buffer capacity INCLUDING the terminating zero, so capacity 64 accepts up to 63 characters. Defaults must fit both the buffer and the selected capacity. Password parameters start with `FLAG_SECURED`; adding access flags preserves it. Default strings and names must have static lifetime, such as string literals or constant global arrays.

```cpp
constexpr EnumOption levels[] = {
    {0, "ERROR"}, {1, "WARN"}, {2, "INFO"}, {3, "DEBUG"}
};

auto level = enumParam("log_level", levels)
    .defaultValue(2).access(FLAG_SERIAL_ACCESS)
    .persistent().tags(TAG_DIAGNOSTICS).build();
```

Enum arrays must use consecutive values starting at zero to match the existing manager and display. Option count is inferred from the array. `access()` adds flags; `persistent(false)` removes the save flag.

`build()` checks numeric default ranges, positive steps, string capacity, and enum definitions using assertions. For guaranteed compile-time checking, declare a definition `constexpr`; an invalid definition then fails compilation. Runtime assertions depend on the build's assertion settings. Builders require C++14 or newer and use the GNU union-initializer syntax already used by this ESP32 project.

The current conversion preserves all 49 parameter indexes, names, types, flags, tags, ranges, steps, and defaults. It does not change the registry checksum inputs or storage format. Add a matching entry to `ParamIndex` when adding a new parameter, as before. Project-specific registry extension is a separate future change; builders alone do not make the registry extensible.
