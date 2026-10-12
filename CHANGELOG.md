# CHANGELOG.md

## v0.2.0 - Library and Project Registration (dev)

- Package shared services in src with Arduino library metadata.
- Allow project-owned parameters and BaseModule instances to register before startup.
- Use runtime registry count in commands, API, MQTT and persistence.
- Keep core parameter IDs stable; application IDs begin at PARAM_COUNT.
- Move credentials and TLS certificate selection to the application.
- First Fermenter source port prepared separately; existing control defects retained.
- Build attempted with ESP32 core 3.3.12, but the compiler could not start due to Windows DLL relocation. Hardware validation pending.



## v0.0.1 — Initial Framework Baseline

### Added
- Modular core architecture
- Param registry system
- Flash storage persistence
- WiFi manager
- MQTT module
- Telnet module
- API module
- Command processor
- Logger module
- Health monitoring
- Watchdog system
- NTP time sync
- Timezone support
- Registry checksum validation
- Auto factory reset on schema mismatch
- Runtime profiling framework

### Planned
- Syslog integration
- OTA firmware updates
- TLS security
- Web dashboard
- Enhanced diagnostics

---