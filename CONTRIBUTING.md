# CONTRIBUTING.md

## Contribution Workflow

### 1. Fork or branch
```bash
git checkout dev
git checkout -b feature/your-feature-name
```

### 2. Coding standards

- Keep modules isolated
- Maintain `BaseModule` compatibility
- Use `ParamManager` for all configurable values
- Avoid hardcoded credentials
- Ensure watchdog safety
- Use logger for diagnostics
- Keep memory usage efficient
- Validate all parameter ranges

### 3. Commit style
```bash
git commit -m "Add syslog module support"
```

### Recommended prefixes
- `Add`
- `Fix`
- `Refactor`
- `Optimize`
- `Remove`
- `Docs`

### 4. Testing requirements

Before merge:
- Clean compile
- Flash persistence test
- Factory reset test
- WiFi reconnection test
- MQTT connectivity test
- Heap stability test
- Long runtime loop test

### 5. Pull Requests

Include:
- Feature summary
- Risks
- Memory impact
- New parameters added
- Registry impact
- Required migration notes

---