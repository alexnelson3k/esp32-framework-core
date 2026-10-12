#ifndef PARAM_BUILDERS_H
#define PARAM_BUILDERS_H

#include "Param_Types.h"
#include <cassert>
#include <cstddef>
#include <limits>

// Builders produce the existing ConfigParam layout; no heap allocation is used.
template<class Derived>
class ParamBuilder {
public:
    constexpr Derived description(const char* text) const {
        Derived next = derived();
        next.param.description = text;
        return next;
    }
    constexpr Derived access(uint16_t flags) const {
        Derived next = derived();
        next.param.flags |= flags;
        return next;
    }
    constexpr Derived tags(uint32_t tags) const {
        Derived next = derived();
        next.param.tags = tags;
        return next;
    }
    constexpr Derived persistent(bool save = true) const {
        Derived next = derived();
        if (save) next.param.flags |= FLAG_SAVE_TO_FLASH;
        else next.param.flags &= ~FLAG_SAVE_TO_FLASH;
        return next;
    }
    constexpr ConfigParam build() const {
        assert(param.name && param.description && derived().valid());
        return param;
    }
protected:
    ConfigParam param;
    constexpr explicit ParamBuilder(ConfigParam value) : param(value) {}
private:
    constexpr const Derived& derived() const {
        return static_cast<const Derived&>(*this);
    }
};

template<class Fields, Fields ConfigParam::*Member>
class NumericParamBuilder : public ParamBuilder<NumericParamBuilder<Fields, Member>> {
    using Base = ParamBuilder<NumericParamBuilder<Fields, Member>>;
    using Value = decltype(Fields::value);
public:
    constexpr explicit NumericParamBuilder(ConfigParam value) : Base(value) {}
    constexpr NumericParamBuilder defaultValue(Value value) const {
        NumericParamBuilder next = *this;
        (next.param.*Member).value = value;
        (next.param.*Member).defaultValue = value;
        return next;
    }
    constexpr NumericParamBuilder range(Value min, Value max) const {
        NumericParamBuilder next = *this;
        (next.param.*Member).min = min;
        (next.param.*Member).max = max;
        return next;
    }
    constexpr NumericParamBuilder step(Value value) const {
        NumericParamBuilder next = *this;
        (next.param.*Member).step = value;
        return next;
    }
    constexpr bool valid() const {
        const Fields& fields = this->param.*Member;
        return fields.min <= fields.max && fields.step > 0 &&
               fields.defaultValue >= fields.min && fields.defaultValue <= fields.max &&
               fields.defaultValue >= std::numeric_limits<Value>::lowest() &&
               fields.defaultValue <= std::numeric_limits<Value>::max();
    }
};

constexpr auto floatParam(const char* name)
    -> NumericParamBuilder<decltype(ConfigParam::floatParam), &ConfigParam::floatParam> {
    return NumericParamBuilder<decltype(ConfigParam::floatParam), &ConfigParam::floatParam>(
        ConfigParam{name, "", TYPE_FLOAT, 0, 0,
            {.floatParam = {0, std::numeric_limits<float>::lowest(),
                std::numeric_limits<float>::max(), 1, 0}}});
}

constexpr auto doubleParam(const char* name)
    -> NumericParamBuilder<decltype(ConfigParam::doubleParam), &ConfigParam::doubleParam> {
    return NumericParamBuilder<decltype(ConfigParam::doubleParam), &ConfigParam::doubleParam>(
        ConfigParam{name, "", TYPE_DOUBLE, 0, 0,
            {.doubleParam = {0, std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::max(), 1, 0}}});
}

constexpr auto uint8Param(const char* name)
    -> NumericParamBuilder<decltype(ConfigParam::uint8Param), &ConfigParam::uint8Param> {
    return NumericParamBuilder<decltype(ConfigParam::uint8Param), &ConfigParam::uint8Param>(
        ConfigParam{name, "", TYPE_UINT8, 0, 0,
            {.uint8Param = {0, std::numeric_limits<uint8_t>::lowest(),
                std::numeric_limits<uint8_t>::max(), 1, 0}}});
}

constexpr auto uint16Param(const char* name)
    -> NumericParamBuilder<decltype(ConfigParam::uint16Param), &ConfigParam::uint16Param> {
    return NumericParamBuilder<decltype(ConfigParam::uint16Param), &ConfigParam::uint16Param>(
        ConfigParam{name, "", TYPE_UINT16, 0, 0,
            {.uint16Param = {0, std::numeric_limits<uint16_t>::lowest(),
                std::numeric_limits<uint16_t>::max(), 1, 0}}});
}

constexpr auto uint32Param(const char* name)
    -> NumericParamBuilder<decltype(ConfigParam::uint32Param), &ConfigParam::uint32Param> {
    return NumericParamBuilder<decltype(ConfigParam::uint32Param), &ConfigParam::uint32Param>(
        ConfigParam{name, "", TYPE_UINT32, 0, 0,
            {.uint32Param = {0, std::numeric_limits<uint32_t>::lowest(),
                std::numeric_limits<uint32_t>::max(), 1, 0}}});
}

constexpr auto int8Param(const char* name)
    -> NumericParamBuilder<decltype(ConfigParam::int8Param), &ConfigParam::int8Param> {
    return NumericParamBuilder<decltype(ConfigParam::int8Param), &ConfigParam::int8Param>(
        ConfigParam{name, "", TYPE_INT8, 0, 0,
            {.int8Param = {0, std::numeric_limits<int8_t>::lowest(),
                std::numeric_limits<int8_t>::max(), 1, 0}}});
}

constexpr auto int16Param(const char* name)
    -> NumericParamBuilder<decltype(ConfigParam::int16Param), &ConfigParam::int16Param> {
    return NumericParamBuilder<decltype(ConfigParam::int16Param), &ConfigParam::int16Param>(
        ConfigParam{name, "", TYPE_INT16, 0, 0,
            {.int16Param = {0, std::numeric_limits<int16_t>::lowest(),
                std::numeric_limits<int16_t>::max(), 1, 0}}});
}

constexpr auto int32Param(const char* name)
    -> NumericParamBuilder<decltype(ConfigParam::int32Param), &ConfigParam::int32Param> {
    return NumericParamBuilder<decltype(ConfigParam::int32Param), &ConfigParam::int32Param>(
        ConfigParam{name, "", TYPE_INT32, 0, 0,
            {.int32Param = {0, std::numeric_limits<int32_t>::lowest(),
                std::numeric_limits<int32_t>::max(), 1, 0}}});
}

class BoolParamBuilder : public ParamBuilder<BoolParamBuilder> {
public:
    constexpr explicit BoolParamBuilder(ConfigParam value) : ParamBuilder(value) {}
    constexpr BoolParamBuilder defaultValue(bool value) const {
        BoolParamBuilder next = *this;
        next.param.boolParam.value = value;
        next.param.boolParam.defaultValue = value;
        return next;
    }
    constexpr bool valid() const { return true; }
};

constexpr BoolParamBuilder boolParam(const char* name) {
    return BoolParamBuilder(ConfigParam{name, "", TYPE_BOOL, 0, 0,
        {.boolParam = {false, false}}});
}

class StringParamBuilder : public ParamBuilder<StringParamBuilder> {
public:
    constexpr explicit StringParamBuilder(ConfigParam value) : ParamBuilder(value) {}
    constexpr StringParamBuilder maxLength(size_t capacity) const {
        StringParamBuilder next = *this;
        next.param.stringParam.maxLength = capacity;
        return next;
    }
    constexpr StringParamBuilder defaultValue(const char* value) const {
        assert(value);
        StringParamBuilder next = *this;
        next.param.stringParam.defaultValue = value;
        size_t i = 0;
        for (; i + 1 < sizeof(next.param.stringParam.value) && value[i]; ++i) {
            next.param.stringParam.value[i] = value[i];
        }
        assert(value[i] == '\0');
        next.param.stringParam.value[i] = '\0';
        return next;
    }
    constexpr bool valid() const {
        size_t capacity = param.stringParam.maxLength;
        if (capacity == 0 || capacity > sizeof(param.stringParam.value) ||
            !param.stringParam.defaultValue) return false;
        for (size_t i = 0; i < capacity; ++i) {
            if (param.stringParam.defaultValue[i] == '\0') return true;
        }
        return false;
    }
};

constexpr StringParamBuilder stringParam(const char* name) {
    return StringParamBuilder(ConfigParam{name, "", TYPE_STRING, 0, 0,
        {.stringParam = {{}, 128, ""}}});
}

constexpr StringParamBuilder passwordParam(const char* name) {
    return StringParamBuilder(ConfigParam{name, "", TYPE_PASSWORD, FLAG_SECURED, 0,
        {.stringParam = {{}, 128, ""}}});
}

class EnumParamBuilder : public ParamBuilder<EnumParamBuilder> {
public:
    constexpr explicit EnumParamBuilder(ConfigParam value) : ParamBuilder(value) {}
    constexpr EnumParamBuilder defaultValue(uint8_t value) const {
        EnumParamBuilder next = *this;
        next.param.enumParam.value = value;
        next.param.enumParam.defaultValue = value;
        return next;
    }
    constexpr bool valid() const {
        if (!param.enumParam.options || param.enumParam.optionCount == 0 ||
            param.enumParam.optionCount > 256 ||
            param.enumParam.defaultValue >= param.enumParam.optionCount) return false;
        // Existing ParamManager and display use enum values as array indexes.
        for (size_t i = 0; i < param.enumParam.optionCount; ++i) {
            if (param.enumParam.options[i].value != i || !param.enumParam.options[i].label)
                return false;
        }
        return true;
    }
};

template<size_t N>
constexpr EnumParamBuilder enumParam(const char* name, const EnumOption (&options)[N]) {
    return EnumParamBuilder(ConfigParam{name, "", TYPE_ENUM, 0, 0,
        {.enumParam = {0, 0, options, N}}});
}

#endif
