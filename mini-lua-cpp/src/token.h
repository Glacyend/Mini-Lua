#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "location.h"

#include <string>
#include <variant>

enum class ReservedType {
    And,
    Break,
    Do,
    Else,
    ElseIf,
    End,
    False,
    For,
    Function,
    If,
    In,
    Local,
    Nil,
    Not,
    Or,
    Repeat,
    Return,
    Then,
    True,
    Until,
    While,
};

struct TokenReserved {
    Location location;
    ReservedType type;

    TokenReserved(Location location, ReservedType type);
};

struct TokenNumber {
    Location location;
    double value;

    TokenNumber(Location location, double value);
};

struct TokenString {
    Location location;
    std::string value;

    TokenString(Location location, std::string value);
};

enum class SymbolType {
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Caret,
    Sharp,
    EqualEqual,
    TildeEqual,
    LessEqual,
    GreaterEqual,
    Less,
    Greater,
    Equal,
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,
    Semicolon,
    Colon,
    Comma,
    Dot,
    DoubleDot,
    TripleDot,
};

struct TokenSymbol {
    Location location;
    SymbolType type;

    TokenSymbol(Location location, SymbolType type);
};

struct TokenName {
    Location location;
    std::string name;

    TokenName(Location location, std::string name);
};

struct TokenEOL {
    Location location;

    TokenEOL(Location location);
};

using Token =
    std::variant<TokenReserved, TokenNumber, TokenString, TokenSymbol, TokenName, TokenEOL>;

Location get_location(const Token& token);

#ifdef DEBUG_MODE

const char* reserved_to_string(ReservedType type);
const char* symbol_to_string(SymbolType type);

template <>
struct std::formatter<Token> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Token& token, FormatContext& ctx) const {
        return std::visit([&](const auto& token) {
            return format_token(token, ctx);
        }, token);
    }

private:
    template <typename FormatContext>
    auto format_token(const TokenReserved& token, FormatContext& ctx) const {
        return std::format_to(
            ctx.out(), "TokenReserved [{}]: {}", token.location, reserved_to_string(token.type)
        );
    }

    template <typename FormatContext>
    auto format_token(const TokenNumber& token, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "TokenNumber [{}]: {:g}", token.location, token.value);
    }

    template <typename FormatContext>
    auto format_token(const TokenString& token, FormatContext& ctx) const {
        std::string raw_string;
        for (char ch : token.value) {
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
        return std::format_to(ctx.out(), "TokenString [{}]: \"{}\"", token.location, raw_string);
    }

    template <typename FormatContext>
    auto format_token(const TokenSymbol& token, FormatContext& ctx) const {
        return std::format_to(
            ctx.out(), "TokenSymbol [{}]: {}", token.location, symbol_to_string(token.type)
        );
    }

    template <typename FormatContext>
    auto format_token(const TokenName& token, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "TokenName [{}]: {}", token.location, token.name);
    }

    template <typename FormatContext>
    auto format_token(const TokenEOL& token, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "TokenEOL [{}]", token.location);
    }
};

#endif

#endif
