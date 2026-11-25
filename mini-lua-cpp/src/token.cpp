#include "token.h"

TokenReserved::TokenReserved(Location location, ReservedType type)
    : location(location),
      type(type) {}

TokenNumber::TokenNumber(Location location, double value)
    : location(location),
      value(value) {}

TokenString::TokenString(Location location, std::string value)
    : location(location),
      value(std::move(value)) {}

TokenSymbol::TokenSymbol(Location location, SymbolType type)
    : location(location),
      type(type) {}

TokenName::TokenName(Location location, std::string name)
    : location(location),
      name(std::move(name)) {}

TokenEOL::TokenEOL(Location location)
    : location(location) {}

Location get_location(const Token& token) {
    return std::visit([](auto&& tok) {
        return tok.location;
    }, token);
}

#ifdef DEBUG_MODE

#include <format>

const char* reserved_to_string(ReservedType type) {
    switch (type) {
        case ReservedType::And: return "and";
        case ReservedType::Break: return "break";
        case ReservedType::Do: return "do";
        case ReservedType::Else: return "else";
        case ReservedType::ElseIf: return "elseif";
        case ReservedType::End: return "end";
        case ReservedType::False: return "false";
        case ReservedType::For: return "for";
        case ReservedType::Function: return "function";
        case ReservedType::If: return "if";
        case ReservedType::In: return "in";
        case ReservedType::Local: return "local";
        case ReservedType::Nil: return "nil";
        case ReservedType::Not: return "not";
        case ReservedType::Or: return "or";
        case ReservedType::Repeat: return "repeat";
        case ReservedType::Return: return "return";
        case ReservedType::Then: return "then";
        case ReservedType::True: return "true";
        case ReservedType::Until: return "until";
        case ReservedType::While: return "while";
    }
}

const char* symbol_to_string(SymbolType type) {
    switch (type) {
        case SymbolType::Plus: return "+";
        case SymbolType::Minus: return "-";
        case SymbolType::Star: return "*";
        case SymbolType::Slash: return "/";
        case SymbolType::Percent: return "%";
        case SymbolType::Caret: return "^";
        case SymbolType::Sharp: return "#";
        case SymbolType::EqualEqual: return "==";
        case SymbolType::TildeEqual: return "~=";
        case SymbolType::LessEqual: return "<=";
        case SymbolType::GreaterEqual: return ">=";
        case SymbolType::Less: return "<";
        case SymbolType::Greater: return ">";
        case SymbolType::Equal: return "=";
        case SymbolType::LeftParen: return "(";
        case SymbolType::RightParen: return ")";
        case SymbolType::LeftBrace: return "{";
        case SymbolType::RightBrace: return "}";
        case SymbolType::LeftBracket: return "[";
        case SymbolType::RightBracket: return "]";
        case SymbolType::Semicolon: return ";";
        case SymbolType::Colon: return ":";
        case SymbolType::Comma: return ",";
        case SymbolType::Dot: return ".";
        case SymbolType::DoubleDot: return "..";
        case SymbolType::TripleDot: return "...";
    }
}

#endif
