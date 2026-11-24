#include "value.h"

#include <utility>

ValueBoolean::ValueBoolean(bool value)
    : value(value) {}

ValueNumber::ValueNumber(double value)
    : value(value) {}

ValueString::ValueString(std::string value)
    : value(std::move(value)) {}
