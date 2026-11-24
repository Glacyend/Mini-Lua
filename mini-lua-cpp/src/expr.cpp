#include "expr.h"

#include <utility>

ExprLiteral::ExprLiteral(Location location, Value value)
    : location(location),
      value(std::move(value)) {}

ExprVariable::ExprVariable(Location location, std::string name)
    : location(location),
      name(std::move(name)) {}

ExprProperty::ExprProperty(Location location, std::unique_ptr<Expr> owner, std::string property)
    : location(location),
      owner(std::move(owner)),
      property(std::move(property)) {}

ExprIndex::ExprIndex(Location location, std::unique_ptr<Expr> owner, std::unique_ptr<Expr> index)
    : location(location),
      owner(std::move(owner)),
      index(std::move(index)) {}

ExprUnary::ExprUnary(Location location, std::unique_ptr<Expr> rhs, Operator ope)
    : location(location),
      rhs(std::move(rhs)),
      ope(ope) {}

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
