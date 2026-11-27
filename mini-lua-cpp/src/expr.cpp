#include "expr.h"

#include <utility>

ExprLiteral::ExprLiteral(Location location, Literal value)
    : location(location),
      value(std::move(value)) {}

ExprVariable::ExprVariable(Location location, std::string name)
    : location(location),
      name(std::move(name)) {}

ExprMemberAccess::ExprMemberAccess(
    Location location, std::unique_ptr<Expr> owner, std::string member_name
)
    : location(location),
      owner(std::move(owner)),
      member_name(std::move(member_name)) {}

ExprIndex::ExprIndex(Location location, std::unique_ptr<Expr> owner, std::unique_ptr<Expr> index)
    : location(location),
      owner(std::move(owner)),
      index(std::move(index)) {}

ExprUnary::ExprUnary(Location location, Operator ope, std::unique_ptr<Expr> rhs)
    : location(location),
      ope(ope),
      rhs(std::move(rhs)) {}

ExprBinary::ExprBinary(
    Location location, std::unique_ptr<Expr> lhs, Operator ope, std::unique_ptr<Expr> rhs
)
    : location(location),
      lhs(std::move(lhs)),
      ope(ope),
      rhs(std::move(rhs)) {}

ExprCall::ExprCall(
    Location location, std::unique_ptr<Expr> callee, std::vector<std::unique_ptr<Expr>> arguments
)
    : location(location),
      callee(std::move(callee)),
      arguments(std::move(arguments)) {}

ExprGrouping::ExprGrouping(Location location, std::unique_ptr<Expr> inner)
    : location(location),
      inner(std::move(inner)) {}

Location get_location(const Expr& expr) {
    return std::visit([](auto&& expr) {
        return expr.location;
    }, expr);
}
