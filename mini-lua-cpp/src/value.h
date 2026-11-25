#ifndef __VALUE_H__
#define __VALUE_H__

#include <string>
#include <variant>

class ValueNil {
public:
    ValueNil() = default;
};

class ValueBoolean {
public:
    bool value;

    ValueBoolean(bool value);
};

class ValueNumber {
public:
    double value;

    ValueNumber(double value);
};

class ValueString {
public:
    std::string value;

    ValueString(std::string value);
};

class ValueFunction {
public:
    // TODO: ObjectFunction

    ValueFunction() = default;
};

class ValueTable {
public:
    // TODO: ObjectTable

    ValueTable() = default;
};

using Value =
    std::variant<ValueNil, ValueBoolean, ValueNumber, ValueString, ValueFunction, ValueTable>;

#ifdef DEBUG_MODE

#include <format>

template <>
struct std::formatter<Value> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Value& value, FormatContext& ctx) const {
        return std::visit([&](const auto& value) {
            return format_value(value, ctx);
        }, value);
    }

private:
    template <typename FormatContext>
    auto format_value([[maybe_unused]] const ValueNil& value, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "ValueNil");
    }

    template <typename FormatContext>
    auto format_value(const ValueBoolean& value, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "ValueBoolean({})", value.value ? "true" : "false");
    }

    template <typename FormatContext>
    auto format_value(const ValueNumber& value, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "ValueNumber({:g})", value.value);
    }

    template <typename FormatContext>
    auto format_value(const ValueString& value, FormatContext& ctx) const {
        std::string raw_string;
        for (char ch : value.value) {
            switch (ch) {
                case '\\': {
                    raw_string += "\\\\";
                    break;
                }
                case '"': {
                    raw_string += "\\\"";
                    break;
                }
                case '\n': {
                    raw_string += "\\n";
                    break;
                }
                default: {
                    raw_string += ch;
                    break;
                }
            }
        }
        return std::format_to(ctx.out(), "ValueString(\"{}\")", raw_string);
    }

    template <typename FormatContext>
    auto format_value([[maybe_unused]] const ValueFunction& value, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "ValueFunction(TODO)");
    }

    template <typename FormatContext>
    auto format_value([[maybe_unused]] const ValueTable& value, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "ValueTable(TODO)");
    }
};

#endif

#endif
