#include "parser.h"
#include "errors.h"
#include "operators.h"
#include "token.h"

#include <type_traits>
#include <utility>
#include <variant>

Parser::Parser(std::vector<Token> tokens)
    : tokens(std::move(tokens)),
      current(0),
      last_location(Position(0, 0), Position(0, 0)) {}

std::unique_ptr<Expr> Parser::parse_expr() {
    return expr_or();
}

std::vector<std::unique_ptr<Expr>> Parser::parse_expressions() {
    std::vector<std::unique_ptr<Expr>> expressions;
    while (!at_end()) {
        while (auto token = peek()) {
            if (std::holds_alternative<TokenEOL>((*token).get())) {
                consume();
            } else {
                break;
            }
        }
        if (at_end()) {
            break;
        }
        expressions.emplace_back(parse_expr());
    }
    return expressions;
}

bool Parser::at_end() const {
    return current >= tokens.size();
}

std::optional<std::reference_wrapper<const Token>> Parser::peek() const {
    if (at_end()) {
        return std::nullopt;
    } else {
        return std::cref(tokens[current]);
    }
}

std::optional<Token> Parser::consume() {
    if (at_end()) {
        return std::nullopt;
    } else {
        last_location = get_location((*peek()).get());
        return std::move(tokens[current++]);
    }
}

Location Parser::end_of_last_token() const {
    return Location(last_location.end, last_location.end);
}

std::unique_ptr<Expr> Parser::expr_or() {
    auto lhs = expr_and();

    while (auto token = peek()) {
        if (auto* reserved = std::get_if<TokenReserved>(&(*token).get());
            reserved != nullptr && reserved->type == ReservedType::Or) {
            consume();
            auto rhs = expr_and();
            lhs = std::make_unique<Expr>(ExprBinary(
                Location::combine(get_location(*lhs), get_location(*rhs)),
                std::move(lhs),
                Operator::Or,
                std::move(rhs)
            ));
        } else {
            break;
        }
    }

    return lhs;
}

std::unique_ptr<Expr> Parser::expr_and() {
    auto lhs = expr_compare();

    while (auto token = peek()) {
        if (auto* reserved = std::get_if<TokenReserved>(&(*token).get());
            reserved != nullptr && reserved->type == ReservedType::And) {
            consume();
            auto rhs = expr_compare();
            lhs = std::make_unique<Expr>(ExprBinary(
                Location::combine(get_location(*lhs), get_location(*rhs)),
                std::move(lhs),
                Operator::And,
                std::move(rhs)
            ));
        } else {
            break;
        }
    }

    return lhs;
}

std::unique_ptr<Expr> Parser::expr_compare() {
    auto lhs = expr_concat();

    while (auto token = peek()) {
        if (auto* symbol = std::get_if<TokenSymbol>(&(*token).get())) {
            std::optional<Operator> ope = std::nullopt;
            switch (symbol->type) {
                case SymbolType::Less: {
                    consume();
                    ope = Operator::LessThan;
                    break;
                }
                case SymbolType::Greater: {
                    consume();
                    ope = Operator::GreaterThan;
                    break;
                }
                case SymbolType::LessEqual: {
                    consume();
                    ope = Operator::LessEqual;
                    break;
                }
                case SymbolType::GreaterEqual: {
                    consume();
                    ope = Operator::GreaterEqual;
                    break;
                }
                case SymbolType::EqualEqual: {
                    consume();
                    ope = Operator::Equal;
                    break;
                }
                case SymbolType::TildeEqual: {
                    consume();
                    ope = Operator::NotEqual;
                    break;
                }
                default: break;
            }
            if (ope.has_value()) {
                auto rhs = expr_concat();
                lhs = std::make_unique<Expr>(ExprBinary(
                    Location::combine(get_location(*lhs), get_location(*rhs)),
                    std::move(lhs),
                    *ope,
                    std::move(rhs)
                ));
            } else {
                break;
            }
        } else {
            break;
        }
    }

    return lhs;
}

std::unique_ptr<Expr> Parser::expr_concat() {
    auto lhs = expr_term();

    if (auto token = peek()) {
        if (auto* symbol = std::get_if<TokenSymbol>(&(*token).get());
            symbol != nullptr && symbol->type == SymbolType::DoubleDot) {
            consume();
            auto rhs = expr_concat();
            lhs = std::make_unique<Expr>(ExprBinary(
                Location::combine(get_location(*lhs), get_location(*rhs)),
                std::move(lhs),
                Operator::Concat,
                std::move(rhs)
            ));
        }
    }

    return lhs;
}

std::unique_ptr<Expr> Parser::expr_term() {
    auto lhs = expr_factor();

    while (auto token = peek()) {
        if (auto* symbol = std::get_if<TokenSymbol>(&(*token).get())) {
            std::optional<Operator> ope = std::nullopt;
            switch (symbol->type) {
                case SymbolType::Plus: {
                    consume();
                    ope = Operator::Plus;
                    break;
                }
                case SymbolType::Minus: {
                    consume();
                    ope = Operator::Minus;
                    break;
                }
                default: break;
            }
            if (ope.has_value()) {
                auto rhs = expr_factor();
                lhs = std::make_unique<Expr>(ExprBinary(
                    Location::combine(get_location(*lhs), get_location(*rhs)),
                    std::move(lhs),
                    *ope,
                    std::move(rhs)
                ));
            } else {
                break;
            }
        } else {
            break;
        }
    }

    return lhs;
}

std::unique_ptr<Expr> Parser::expr_factor() {
    auto lhs = expr_unary();

    while (auto token = peek()) {
        if (auto* symbol = std::get_if<TokenSymbol>(&(*token).get())) {
            std::optional<Operator> ope = std::nullopt;
            switch (symbol->type) {
                case SymbolType::Star: {
                    consume();
                    ope = Operator::Multiply;
                    break;
                }
                case SymbolType::Slash: {
                    consume();
                    ope = Operator::Divide;
                    break;
                }
                case SymbolType::Percent: {
                    consume();
                    ope = Operator::Mod;
                    break;
                }
                default: break;
            }
            if (ope.has_value()) {
                auto rhs = expr_unary();
                lhs = std::make_unique<Expr>(ExprBinary(
                    Location::combine(get_location(*lhs), get_location(*rhs)),
                    std::move(lhs),
                    *ope,
                    std::move(rhs)
                ));
            } else {
                break;
            }
        } else {
            break;
        }
    }

    return lhs;
}

std::unique_ptr<Expr> Parser::expr_unary() {
    std::optional<Operator> ope = std::nullopt;
    std::optional<Location> ope_location = std::nullopt;

    if (auto token = peek()) {
        if (auto* reserved = std::get_if<TokenReserved>(&(*token).get());
            reserved != nullptr && reserved->type == ReservedType::Not) {
            consume();
            ope_location = last_location;
            ope = Operator::Not;
        } else if (auto* symbol = std::get_if<TokenSymbol>(&(*token).get())) {
            if (symbol->type == SymbolType::Minus) {
                consume();
                ope_location = last_location;
                ope = Operator::Negative;
            } else if (symbol->type == SymbolType::Sharp) {
                consume();
                ope_location = last_location;
                ope = Operator::Length;
            }
        }
    }

    if (ope.has_value()) {
        auto operand = expr_unary();
        return std::make_unique<Expr>(ExprUnary(
            Location::combine(*ope_location, get_location(*operand)), *ope, std::move(operand)
        ));
    } else {
        return expr_power();
    }
}

std::unique_ptr<Expr> Parser::expr_power() {
    auto lhs = expr_suffix();

    if (auto token = peek()) {
        if (auto* symbol = std::get_if<TokenSymbol>(&(*token).get());
            symbol != nullptr && symbol->type == SymbolType::Caret) {
            consume();
            auto rhs = expr_unary();
            lhs = std::make_unique<Expr>(ExprBinary(
                Location::combine(get_location(*lhs), get_location(*rhs)),
                std::move(lhs),
                Operator::Power,
                std::move(rhs)
            ));
        }
    }

    return lhs;
}

std::unique_ptr<Expr> Parser::expr_member_access(std::unique_ptr<Expr> owner) {
    if (auto name_token = consume()) {
        if (auto* name = std::get_if<TokenName>(&*name_token)) {
            return std::make_unique<Expr>(ExprMemberAccess(
                Location::combine(get_location(*owner), name->location),
                std::move(owner),
                std::move(name->name)
            ));
        } else {
            throw SyntaxError("Expected a member name.", last_location);
        }
    } else {
        throw SyntaxError("Expected a member name.", end_of_last_token());
    }
}

std::unique_ptr<Expr> Parser::expr_index(std::unique_ptr<Expr> owner) {
    auto index = parse_expr();
    if (auto closing = consume()) {
        if (auto* closing_symbol = std::get_if<TokenSymbol>(&*closing);
            closing_symbol != nullptr && closing_symbol->type == SymbolType::RightBracket) {
            return std::make_unique<Expr>(ExprIndex(
                Location::combine(get_location(*owner), closing_symbol->location),
                std::move(owner),
                std::move(index)
            ));
        } else {
            throw SyntaxError("Expected a closing bracket.", last_location);
        }
    } else {
        throw SyntaxError("Unterminated index.", last_location);
    }
}

std::unique_ptr<Expr> Parser::expr_call(std::unique_ptr<Expr> callee) {
    std::vector<std::unique_ptr<Expr>> arguments;
    bool found_closing = false;
    bool need_comma = false;
    while (auto next_token = peek()) {
        if (auto* token_symbol = std::get_if<TokenSymbol>(&(*next_token).get())) {
            if (token_symbol->type == SymbolType::RightParen) {
                found_closing = true;
                consume();
                break;
            } else if (token_symbol->type == SymbolType::Comma) {
                if (!need_comma) {
                    throw SyntaxError("Expected an argument.", token_symbol->location);
                }
                consume();
                need_comma = false;
            }
        }
        if (need_comma) {
            throw SyntaxError("Expected a comma.", get_location(*next_token));
        }
        arguments.emplace_back(parse_expr());
        need_comma = true;
    }
    if (!found_closing) {
        throw SyntaxError("Expected a closing parenthesis.", end_of_last_token());
    }
    return std::make_unique<Expr>(ExprCall(
        Location::combine(get_location(*callee), last_location),
        std::move(callee),
        std::move(arguments)
    ));
}

std::unique_ptr<Expr> Parser::expr_suffix() {
    auto lhs = expr_primary();

    while (auto token = peek()) {
        if (auto* symbol = std::get_if<TokenSymbol>(&(*token).get())) {
            bool break_while = false;
            switch (symbol->type) {
                case SymbolType::Dot: {
                    consume();
                    lhs = expr_member_access(std::move(lhs));
                    break;
                }
                case SymbolType::LeftBracket: {
                    consume();
                    lhs = expr_index(std::move(lhs));
                    break;
                }
                case SymbolType::LeftParen: {
                    consume();
                    lhs = expr_call(std::move(lhs));
                    break;
                }
                default: {
                    break_while = true;
                    break;
                }
            }
            if (break_while) {
                break;
            }
        } else {
            break;
        }
    }

    return lhs;
}

std::unique_ptr<Expr> Parser::expr_primary() {
    if (at_end()) {
        throw SyntaxError("Expected an expression.", end_of_last_token());
    }

    Token token = *consume();
    return std::visit([&]<typename T>(T& token) -> std::unique_ptr<Expr> {
        if constexpr (std::is_same_v<T, TokenNumber>) {
            return std::make_unique<Expr>(ExprLiteral(token.location, LiteralNumber(token.value)));
        } else if constexpr (std::is_same_v<T, TokenString>) {
            return std::make_unique<Expr>(
                ExprLiteral(token.location, LiteralString(std::move(token.value)))
            );
        } else if constexpr (std::is_same_v<T, TokenReserved>) {
            switch (token.type) {
                case ReservedType::Nil: {
                    return std::make_unique<Expr>(ExprLiteral(token.location, LiteralNil()));
                }
                case ReservedType::True: {
                    return std::make_unique<Expr>(
                        ExprLiteral(token.location, LiteralBoolean(true))
                    );
                }
                case ReservedType::False: {
                    return std::make_unique<Expr>(
                        ExprLiteral(token.location, LiteralBoolean(false))
                    );
                }
                default: {
                    throw SyntaxError("Expected an expression.", token.location);
                }
            }
        } else if constexpr (std::is_same_v<T, TokenSymbol>) {
            if (token.type == SymbolType::LeftBrace) {
                if (auto closing = consume()) {
                    if (auto* closing_symbol = std::get_if<TokenSymbol>(&*closing);
                        closing_symbol != nullptr
                        && closing_symbol->type == SymbolType::RightBrace) {
                        return std::make_unique<Expr>(ExprLiteral(
                            Location::combine(token.location, closing_symbol->location),
                            LiteralEmptyTable()
                        ));
                    } else {
                        throw SyntaxError("Only empty tables are supported.", last_location);
                    }
                } else {
                    throw SyntaxError("Unterminated table.", end_of_last_token());
                }
            } else if (token.type == SymbolType::LeftParen) {
                auto inner = parse_expr();
                if (auto closing = consume()) {
                    if (auto* closing_symbol = std::get_if<TokenSymbol>(&*closing);
                        closing_symbol != nullptr
                        && closing_symbol->type == SymbolType::RightParen) {
                        return std::make_unique<Expr>(ExprGrouping(
                            Location::combine(token.location, closing_symbol->location),
                            std::move(inner)
                        ));
                    } else {
                        throw SyntaxError("Expected a closing parenthesis.", last_location);
                    }
                } else {
                    throw SyntaxError("Unterminated parenthesis.", end_of_last_token());
                }
            } else {
                throw SyntaxError("Expected an expression.", token.location);
            }
        } else if constexpr (std::is_same_v<T, TokenName>) {
            return std::make_unique<Expr>(ExprVariable(token.location, std::move(token.name)));
        } else if constexpr (std::is_same_v<T, TokenEOL>) {
            throw SyntaxError("Expected an expression.", token.location);
        }
    }, token);
}
