#include "Param_Types.h"
#include <cstring>
#include <vector>

namespace {
bool sealed = false;
std::vector<ConfigParam>& projectRegistry() {
    static std::vector<ConfigParam> registry;
    return registry;
}
}

void sealParameterRegistry() {
    sealed = true;
}

bool registerProjectParameters(const ConfigParam* parameters, size_t count) {
    if (sealed || !parameters || count == 0 ||
        count > UINT16_MAX - system_param_count) return false;

    for (size_t i = 0; i < count; ++i) {
        if (!parameters[i].name || !parameters[i].description) return false;
        for (uint16_t j = 0; j < system_param_count; ++j) {
            if (strcmp(parameters[i].name, system_params[j].name) == 0) return false;
        }
        for (size_t j = 0; j < i; ++j) {
            if (strcmp(parameters[i].name, parameters[j].name) == 0) return false;
        }
    }

    auto& registry = projectRegistry();
    if (registry.empty()) {
        registry.assign(system_params, system_params + system_param_count);
    }
    registry.insert(registry.end(), parameters, parameters + count);
    system_params = registry.data();
    system_param_count = static_cast<uint16_t>(registry.size());
    return true;
}
