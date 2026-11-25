#ifndef __EXPR_H__
#define __EXPR_H__

#include "location.h"
#include "operators.h"
#include "value.h"

#include <memory>
#include <string>
#include <variant>
#include <vector>

class ExprLiteral;
class ExprVariable;
class ExprMemberAccess;
class ExprIndex;
class ExprUnary;
class ExprBinary;
class ExprCall;
class ExprGrouping;

using Expr = std::variant<
    ExprLiteral,
    ExprVariable,
    ExprMemberAccess,
    ExprIndex,
    ExprUnary,
    ExprBinary,
    ExprCall,
    ExprGrouping>;

class ExprLiteral {
public:
    Location location;
    Value value;

    ExprLiteral(Location location, Value value);
};

class ExprVariable {
public:
    Location location;
    std::string name;

    ExprVariable(Location location, std::string name);
};

class ExprMemberAccess {
public:
    Location location;
    std::unique_ptr<Expr> owner;
    std::string member_name;

    ExprMemberAccess(Location location, std::unique_ptr<Expr> owner, std::string member_name);
};

class ExprIndex {
public:
    Location location;
    std::unique_ptr<Expr> owner;
    std::unique_ptr<Expr> index;

    ExprIndex(Location location, std::unique_ptr<Expr> owner, std::unique_ptr<Expr> index);
};

class ExprUnary {
public:
    Location location;
    Operator ope;
    std::unique_ptr<Expr> rhs;

    ExprUnary(Location location, Operator ope, std::unique_ptr<Expr> rhs);
};

class ExprBinary {
public:
    Location location;
    std::unique_ptr<Expr> lhs;
    Operator ope;
    std::unique_ptr<Expr> rhs;

    ExprBinary(
        Location location, std::unique_ptr<Expr> lhs, Operator ope, std::unique_ptr<Expr> rhs
    );
};

class ExprCall {
public:
    Location location;
    std::unique_ptr<Expr> callee;
    std::vector<std::unique_ptr<Expr>> arguments;

    ExprCall(
        Location location,
        std::unique_ptr<Expr> callee,
        std::vector<std::unique_ptr<Expr>> arguments
    );
};

class ExprGrouping {
public:
    Location location;
    std::unique_ptr<Expr> inner;

    ExprGrouping(Location location, std::unique_ptr<Expr> inner);
};

Location get_location(const Expr& expr);

#ifdef DEBUG_MODE

#include <cstddef>
#include <format>

static inline std::string indent(std::size_t level) {
    return std::string(level * 4, ' ');
}

static inline std::string to_string(const Expr& expr, std::size_t level);

static inline std::string expr_to_string(
    const ExprLiteral& expr, [[maybe_unused]] std::size_t level
) {
    return std::format("ExprLiteral [{}]: {}", expr.location, expr.value);
}

static inline std::string expr_to_string(
    const ExprVariable& expr, [[maybe_unused]] std::size_t level
) {
    return std::format("ExprVariable [{}]: {}", expr.location, expr.name);
}

static inline std::string expr_to_string(const ExprMemberAccess& expr, std::size_t level) {
    std::string result = std::format("ExprMemberAccess [{}]: {{", expr.location);
    result += std::format("\n{}    owner: ", indent(level));
    result += to_string(*expr.owner, level + 1);
    result += std::format("\n{}    member_name: {}", indent(level), expr.member_name);
    result += std::format("\n{}}}", indent(level));
    return result;
}

static inline std::string expr_to_string(const ExprIndex& expr, std::size_t level) {
    std::string result = std::format("ExprIndex [{}]: {{", expr.location);
    result += std::format("\n{}    owner: ", indent(level));
    result += to_string(*expr.owner, level + 1);
    result += std::format("\n{}    index: ", indent(level));
    result += to_string(*expr.index, level + 1);
    result += std::format("\n{}}}", indent(level));
    return result;
}

static inline std::string expr_to_string(const ExprUnary& expr, std::size_t level) {
    std::string result = std::format("ExprUnary [{}]: {{", expr.location);
    result += std::format("\n{}    ope: {}", indent(level), operator_to_string(expr.ope));
    result += std::format("\n{}    rhs: ", indent(level));
    result += to_string(*expr.rhs, level + 1);
    result += std::format("\n{}}}", indent(level));
    return result;
}

static inline std::string expr_to_string(const ExprBinary& expr, std::size_t level) {
    std::string result = std::format("ExprBinary [{}]: {{", expr.location);
    result += std::format("\n{}    ope: {}", indent(level), operator_to_string(expr.ope));
    result += std::format("\n{}    lhs: ", indent(level));
    result += to_string(*expr.lhs, level + 1);
    result += std::format("\n{}    rhs: ", indent(level));
    result += to_string(*expr.rhs, level + 1);
    result += std::format("\n{}}}", indent(level));
    return result;
}

static inline std::string expr_to_string(const ExprCall& expr, std::size_t level) {
    std::string result = std::format("ExprCall [{}]: {{", expr.location);
    result += std::format("\n{}    callee: ", indent(level));
    result += to_string(*expr.callee, level + 1);
    result += std::format("\n{}    arguments: [", indent(level));
    if (!expr.arguments.empty()) {
        for (const auto& arg : expr.arguments) {
            result += std::format("\n{}    {},", indent(level + 1), to_string(*arg, level + 2));
        }
        result += std::format("\n{}    ]", indent(level));
    } else {
        result += "]";
    }
    result += std::format("\n{}}}", indent(level));
    return result;
}

static inline std::string expr_to_string(const ExprGrouping& expr, std::size_t level) {
    std::string result = std::format("ExprGrouping [{}]: {{", expr.location);
    result += std::format("\n{}    inner: ", indent(level));
    result += to_string(*expr.inner, level + 1);
    result += std::format("\n{}}}", indent(level));
    return result;
}

static inline std::string to_string(const Expr& expr, std::size_t level) {
    return std::visit([&level](const auto& expr) {
        return expr_to_string(expr, level);
    }, expr);
}

template <>
struct std::formatter<Expr> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Expr& expr, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "{}", to_string(expr, 0));
    }
};

#endif

#endif
