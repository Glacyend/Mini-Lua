#ifndef __LITERALS_H__
#define __LITERALS_H__

#include <string>
#include <variant>

class LiteralNil {
public:
    LiteralNil() = default;
};

class LiteralBoolean {
public:
    bool value;

    LiteralBoolean(bool value);
};

class LiteralNumber {
public:
    double value;

    LiteralNumber(double value);
};

class LiteralString {
public:
    std::string value;

    LiteralString(std::string value);
};

class LiteralEmptyTable {
public:
    LiteralEmptyTable() = default;
};

using Literal =
    std::variant<LiteralNil, LiteralBoolean, LiteralNumber, LiteralString, LiteralEmptyTable>;

#ifdef DEBUG_MODE

#include <format>

template <>
struct std::formatter<Literal> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Literal& literal, FormatContext& ctx) const {
        return std::visit([&](const auto& value) {
            return format_literal(value, ctx);
        }, literal);
    }

private:
    template <typename FormatContext>
    auto format_literal([[maybe_unused]] const LiteralNil& literal, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "LiteralNil");
    }

    template <typename FormatContext>
    auto format_literal(const LiteralBoolean& literal, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "LiteralBoolean({})", literal.value ? "true" : "false");
    }

    template <typename FormatContext>
    auto format_literal(const LiteralNumber& literal, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "LiteralNumber({:g})", literal.value);
    }

    template <typename FormatContext>
    auto format_literal(const LiteralString& literal, FormatContext& ctx) const {
        std::string raw_string;
        for (char ch : literal.value) {
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
    auto format_literal(
        [[maybe_unused]] const LiteralEmptyTable& literal, FormatContext& ctx
    ) const {
        return std::format_to(ctx.out(), "LiteralEmptyTable");
    }
};

#endif

#endif
