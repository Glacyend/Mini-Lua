#ifndef __PARSER_H__
#define __PARSER_H__

#include "expr.h"
#include "location.h"
#include "token.h"

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

class Parser {
private:
    std::vector<Token> tokens;
    std::size_t current;
    Location last_location;

    bool at_end() const;
    std::optional<std::reference_wrapper<const Token>> peek() const;
    std::optional<Token> consume();
    Location end_of_last_token() const;
    std::unique_ptr<Expr> expr_or();
    std::unique_ptr<Expr> expr_and();
    std::unique_ptr<Expr> expr_compare();
    std::unique_ptr<Expr> expr_concat();
    std::unique_ptr<Expr> expr_term();
    std::unique_ptr<Expr> expr_factor();
    std::unique_ptr<Expr> expr_unary();
    std::unique_ptr<Expr> expr_power();
    std::unique_ptr<Expr> expr_member_access(std::unique_ptr<Expr> owner);
    std::unique_ptr<Expr> expr_index(std::unique_ptr<Expr> owner);
    std::unique_ptr<Expr> expr_call(std::unique_ptr<Expr> callee);
    std::unique_ptr<Expr> expr_suffix();
    std::unique_ptr<Expr> expr_primary();
    std::unique_ptr<Expr> parse_expr();

public:
    Parser(std::vector<Token> tokens);
    std::vector<std::unique_ptr<Expr>> parse_expressions();
};

#endif
