#include "literals.h"

#include <utility>

LiteralBoolean::LiteralBoolean(bool value)
    : value(value) {}

LiteralNumber::LiteralNumber(double value)
    : value(value) {}

LiteralString::LiteralString(std::string value)
    : value(std::move(value)) {}
