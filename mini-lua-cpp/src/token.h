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

#endif
