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
class ExprProperty;
class ExprIndex;
class ExprUnary;
class ExprBinary;
class ExprCall;

using Expr = std::
    variant<ExprLiteral, ExprVariable, ExprProperty, ExprIndex, ExprUnary, ExprBinary, ExprCall>;

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

class ExprProperty {
public:
    Location location;
    std::unique_ptr<Expr> owner;
    std::string property;

    ExprProperty(Location location, std::unique_ptr<Expr> owner, std::string property);
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
    std::unique_ptr<Expr> rhs;
    Operator ope;

    ExprUnary(Location location, std::unique_ptr<Expr> rhs, Operator ope);
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

#endif
