#include "scanner.h"

#include "errors.h"

#include <cctype>
#include <cmath>
#include <iterator>
#include <limits>
#include <utility>

Scanner::Scanner(std::string_view source)
    : source(source),
      token_start(source.begin()),
      current(source.begin()),
      line_start(source.begin()),
      tokens(),
      line_offset({ 0 }) {}

void Scanner::scan_tokens() {
    while (!at_end()) {
        scan_token();
    }
}

std::pair<std::vector<Token>, std::vector<std::size_t>> Scanner::get_result() {
    return { std::move(tokens), std::move(line_offset) };
}

bool Scanner::at_end() const {
    return current == source.end();
}

char Scanner::peek() const {
    if (at_end()) {
        return '\0';
    } else {
        return *current;
    }
}

char Scanner::consume() {
    if (at_end()) {
        return '\0';
    } else {
        return *current++;
    }
}

char Scanner::last_consumed() const {
    if (current == source.begin()) {
        return '\0';
    } else {
        return *std::prev(current);
    }
}

void Scanner::skip_comment() {
    while (!at_end() && peek() != '\n') {
        consume();
    }
}

void Scanner::skip_whitespace() {
    while (!at_end() && std::isspace(peek()) && peek() != '\n') {
        consume();
    }
}

bool Scanner::match_consume(char expected) {
    if (at_end() || peek() != expected) {
        return false;
    } else {
        consume();
        return true;
    }
}

void Scanner::scan_token() {
    skip_whitespace();
    align_pointer();

    char ch = consume();
    switch (ch) {
        case '\n': {
            new_line();
            break;
        }
        case '+': {
            make_token_symbol(SymbolType::Plus);
            break;
        }
        case '-': {
            if (match_consume('-')) {
                skip_comment();
            } else {
                make_token_symbol(SymbolType::Minus);
            }
            break;
        }
        case '*': {
            make_token_symbol(SymbolType::Star);
            break;
        }
        case '/': {
            make_token_symbol(SymbolType::Slash);
            break;
        }
        case '%': {
            make_token_symbol(SymbolType::Percent);
            break;
        }
        case '^': {
            make_token_symbol(SymbolType::Caret);
            break;
        }
        case '#': {
            make_token_symbol(SymbolType::Sharp);
            break;
        }
        case '=': {
            make_token_symbol(match_consume('=') ? SymbolType::EqualEqual : SymbolType::Equal);
            break;
        }
        case '~': {
            if (match_consume('=')) {
                make_token_symbol(SymbolType::TildeEqual);
            } else {
                throw LexicalError("Expected '=' after '~'.", get_token_location());
            }
            break;
        }
        case '<': {
            make_token_symbol(match_consume('=') ? SymbolType::LessEqual : SymbolType::Less);
            break;
        }
        case '>': {
            make_token_symbol(match_consume('=') ? SymbolType::GreaterEqual : SymbolType::Greater);
            break;
        }
        case '(': {
            make_token_symbol(SymbolType::LeftParen);
            break;
        }
        case ')': {
            make_token_symbol(SymbolType::RightParen);
            break;
        }
        case '{': {
            make_token_symbol(SymbolType::LeftBrace);
            break;
        }
        case '}': {
            make_token_symbol(SymbolType::RightBrace);
            break;
        }
        case '[': {
            make_token_symbol(SymbolType::LeftBracket);
            break;
        }
        case ']': {
            make_token_symbol(SymbolType::RightBracket);
            break;
        }
        case ';': {
            make_token_symbol(SymbolType::Semicolon);
            break;
        }
        case ':': {
            make_token_symbol(SymbolType::Colon);
            break;
        }
        case ',': {
            make_token_symbol(SymbolType::Comma);
            break;
        }
        case '.': {
            if (match_consume('.')) {
                make_token_symbol(
                    match_consume('.') ? SymbolType::TripleDot : SymbolType::DoubleDot
                );
            } else if (std::isdigit(peek())) {
                scan_number();
            } else {
                make_token_symbol(SymbolType::Dot);
            }
            break;
        }
        case '\'':
        case '"': {
            scan_string();
            break;
        }
        default: {
            if (std::isalpha(ch) || ch == '_') {
                scan_identifier();
            } else if (std::isdigit(ch)) {
                scan_number();
            } else {
                throw LexicalError("Unexpected character.", get_token_location());
            }
        }
    }
}

void Scanner::new_line() {
    make_token_eol();
    line_start = current;
    line_offset.push_back(static_cast<std::size_t>(line_start - source.begin()));
}

void Scanner::scan_identifier() {
    while (std::isalnum(peek()) || peek() == '_') {
        consume();
    }

    std::string_view identifier(token_start, static_cast<std::size_t>(current - token_start));
    if (auto it = reserved_keywords.find(identifier); it != reserved_keywords.end()) {
        make_token_reserved(it->second);
    } else {
        make_token_name(std::string(identifier));
    }
}

void Scanner::scan_number() {
    double value = 0.0;

    if (last_consumed() == '0' && (match_consume('x') || match_consume('X'))) {
        auto is_hex_digit = [](char ch) {
            return std::isdigit(ch) || ('a' <= ch && ch <= 'f') || ('A' <= ch && ch <= 'F');
        };

        while (!at_end() && is_hex_digit(peek())) {
            char ch = consume();
            if ('0' <= ch && ch <= '9') {
                value = value * 16.0 + static_cast<double>(ch - '0');
            } else {
                double digit = 0.0;
                switch (ch) {
                    case 'a':
                    case 'A': {
                        digit = 10.0;
                        break;
                    }
                    case 'b':
                    case 'B': {
                        digit = 11.0;
                        break;
                    }
                    case 'c':
                    case 'C': {
                        digit = 12.0;
                        break;
                    }
                    case 'd':
                    case 'D': {
                        digit = 13.0;
                        break;
                    }
                    case 'e':
                    case 'E': {
                        digit = 14.0;
                        break;
                    }
                    case 'f':
                    case 'F': {
                        digit = 15.0;
                        break;
                    }
                }
                value = value * 16.0 + digit;
            }
        }
        if (std::isalpha(peek())) {
            throw LexicalError("Invalid hexadecimal number.", get_token_location());
        }

        make_token_number(value);
        return;
    }

    bool has_integer_part = false;
    if (last_consumed() != '.') {
        has_integer_part = true;
        value = static_cast<double>(last_consumed() - '0');
        while (!at_end() && std::isdigit(peek())) {
            char ch = consume();
            value = value * 10.0 + static_cast<double>(ch - '0');
        }
        if (peek() == '.') {
            consume();
        }
    }

    if (last_consumed() == '.') {
        double divisor = 10.0;
        if (!std::isdigit(peek()) && !has_integer_part) {
            throw LexicalError(
                "Numbers without an integer part must have a fractional part.", get_token_location()
            );
        }
        while (!at_end() && std::isdigit(peek())) {
            char ch = consume();
            double digit = ch - '0';
            value += digit / divisor;
            divisor *= 10.0;
        }
    }

    if (match_consume('e') || match_consume('E')) {
        bool negative = false;
        long long exp = 0;
        if (match_consume('-')) {
            negative = true;
        } else {
            match_consume('+');
        }
        if (!std::isdigit(peek())) {
            throw LexicalError("Expected the exponent part.", get_token_location());
        }
        while (!at_end() && std::isdigit(peek())) {
            char ch = consume();
            long long digit = ch - '0';
            if (exp > (std::numeric_limits<long long>::max() - digit) / 10) {
                throw LexicalError("Exponent overflow.", get_token_location());
            }
            exp = exp * 10 + digit;
        }
        if (negative) {
            exp = -exp;
        }
        value *= std::pow(10.0, static_cast<double>(exp));
    }

    make_token_number(value);
}

void Scanner::scan_string() {
    char delimiter = last_consumed();
    std::string value;

    while (!at_end() && peek() != delimiter) {
        if (peek() == '\n') {
            throw LexicalError("Unterminated string.", get_location(current, std::next(current)));
        }
        
        if (match_consume('\\')) {
            if (at_end()) {
                throw LexicalError(
                    "Unterminated string.", get_location(std::prev(current), current)
                );
            }
            if (match_consume('"')) {
                value.push_back('"');
            } else if (match_consume('\'')) {
                value.push_back('\'');
            } else if (match_consume('\\')) {
                value.push_back('\\');
            } else if (match_consume('n')) {
                value.push_back('\n');
            } else {
                throw LexicalError(
                    "Invalid escape sequence.", get_location(std::prev(current), std::next(current))
                );
            }
        } else {
            value.push_back(consume());
        }
    }

    if (at_end()) {
        throw LexicalError("Unterminated string.", get_location(std::prev(current), current));
    }

    consume();  // delimiter
    make_token_string(value);
}

void Scanner::align_pointer() {
    token_start = current;
}

Location Scanner::get_location(
    std::string_view::const_iterator start, std::string_view::const_iterator end
) const {
    return Location(
        Position(line_offset.size() - 1, static_cast<std::size_t>(start - line_start)),
        Position(line_offset.size() - 1, static_cast<std::size_t>(end - line_start))
    );
}

Location Scanner::get_token_location() const {
    return get_location(token_start, current);
}

void Scanner::make_token_reserved(ReservedType type) {
    tokens.push_back(TokenReserved(get_token_location(), type));
}

void Scanner::make_token_number(double value) {
    tokens.push_back(TokenNumber(get_token_location(), value));
}

void Scanner::make_token_string(std::string value) {
    tokens.push_back(TokenString(get_token_location(), std::move(value)));
}

void Scanner::make_token_symbol(SymbolType type) {
    tokens.push_back(TokenSymbol(get_token_location(), type));
}

void Scanner::make_token_name(std::string name) {
    tokens.push_back(TokenName(get_token_location(), std::move(name)));
}

void Scanner::make_token_eol() {
    tokens.push_back(TokenEOL(get_token_location()));
}
