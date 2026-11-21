#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "token.h"

#include <cstddef>
#include <map>
#include <string_view>
#include <utility>
#include <vector>

class Scanner {
private:
    static inline const std::map<std::string_view, ReservedType> reserved_keywords = {
        { "and", ReservedType::And },
        { "break", ReservedType::Break },
        { "do", ReservedType::Do },
        { "else", ReservedType::Else },
        { "elseif", ReservedType::ElseIf },
        { "end", ReservedType::End },
        { "false", ReservedType::False },
        { "for", ReservedType::For },
        { "function", ReservedType::Function },
        { "if", ReservedType::If },
        { "in", ReservedType::In },
        { "local", ReservedType::Local },
        { "nil", ReservedType::Nil },
        { "not", ReservedType::Not },
        { "or", ReservedType::Or },
        { "repeat", ReservedType::Repeat },
        { "return", ReservedType::Return },
        { "then", ReservedType::Then },
        { "true", ReservedType::True },
        { "until", ReservedType::Until },
        { "while", ReservedType::While },
    };

    std::string_view source;
    std::string_view::const_iterator token_start;
    std::string_view::const_iterator current;
    std::string_view::const_iterator line_start;
    std::vector<Token> tokens;
    std::vector<std::size_t> line_offset;

    bool at_end() const;
    char peek() const;
    char consume();
    char last_consumed() const;
    void skip_comment();
    void skip_whitespace();
    bool match_consume(char expected);
    void scan_token();
    void new_line();
    void scan_identifier();
    void scan_number();
    void scan_string();
    void align_pointer();
    Location get_location(
        std::string_view::const_iterator start, std::string_view::const_iterator end
    ) const;
    Location get_token_location() const;
    void make_token_reserved(ReservedType type);
    void make_token_number(double value);
    void make_token_string(std::string value);
    void make_token_symbol(SymbolType type);
    void make_token_name(std::string name);
    void make_token_eol();

public:
    Scanner(std::string_view source);
    void scan_tokens();
    std::pair<std::vector<Token>, std::vector<std::size_t>> get_result();
};

#endif
