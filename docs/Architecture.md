# docs/Architecture.md

# System Architecture

## Boot Sequence

```text
setup()
 ├── initializeSystem()
 ├── registerModules()
 ├── storageManager.init()
 ├── paramManager.init()
 ├── timeManager.init()
 ├── loggerModule.init()
 └── moduleManager.startModules()
```

---

## Main Loop

```text
loop()
 ├── loggerModule.loop()
 ├── timeManager.loop()
 ├── watchdogModule.loop()
 ├── processCoreTasks()
 ├── moduleManager.loopModules()
 ├── eventBus.process()
 └── storageManager.process()
```

---

## Parameter Flow

```text
Registry Definition
    ↓
ParamManager Validation
    ↓
StorageManager Persistence
    ↓
MQTT / API / CLI Access
```

---

## Safety Layers

### Layer 1
Registry checksum validation

### Layer 2
Schema ID validation

### Layer 3
Watchdog monitoring

### Layer 4
Health reporting

### Layer 5
Factory reset recovery

---

## Logging Layers

### Serial
Development diagnostics

### MQTT
Remote fleet diagnostics

### Syslog (planned)
Enterprise centralized logging

---

## Expansion Strategy

Future modules should:

- Inherit `BaseModule`
- Register with `ModuleManager`
- Use `ParamManager`
- Use `LoggerModule`
- Support health diagnostics
- Support profiling

---

# Recommended Git Branch Strategy

## Main branches

### main
Production stable releases

### dev
Integration branch

### feature/*
Module development

### hotfix/*
Critical fixes

---

# Release Strategy

## Versioning

- `v0.x.x` → active framework development
- `v1.0.0` → production-ready stable platform
- `v2.x.x` → enterprise feature expansion

---

# Immediate Next Priorities

## High Priority
- Stabilize current build
- Resolve module visibility issues
- Finalize command system
- Add public diagnostics APIs
- Complete syslog backend

## Medium Priority
- OTA
- TLS MQTT
- Dashboard
- Advanced auth

## Long Term
- Commercial-grade deployment tools
- Fleet management
- Device provisioning
- CI/CD firmware pipeline

---

# Professional Repository Structure

```text
ESP32-Framework-Core/
│
├── src/
├── docs/
│   └── Architecture.md
├── README.md
├── CHANGELOG.md
├── CONTRIBUTING.md
├── LICENSE
└── platformio.ini / Arduino project files
```

---