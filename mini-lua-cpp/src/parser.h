#ifndef __PARSER_H__
#define __PARSER_H__

#include "common.h"
#include "expr.h"
#include "token.h"

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

class Parser {
private:
    std::vector<Token> tokens;
    std::size_t current;

    bool at_end() const;
    std::optional<Token> peek() const;
    std::optional<Token> consume();
    template <variant_alternative_of<Token> T>
    bool match_consume();

public:
    Parser(std::vector<Token> tokens);
    std::unique_ptr<Expr> parse_expr();
};

#endif
