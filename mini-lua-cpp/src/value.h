#ifndef __VALUE_H__
#define __VALUE_H__

#include <string>
#include <variant>

class ValueNil {
public:
    ValueNil() = default;
};

class ValueBoolean {
public:
    bool value;

    ValueBoolean(bool value);
};

class ValueNumber {
public:
    double value;

    ValueNumber(double value);
};

class ValueString {
public:
    std::string value;

    ValueString(std::string value);
};

class ValueFunction {
public:
    // TODO: ObjectFunction

    ValueFunction() = default;
};

class ValueTable {
public:
    // TODO: ObjectTable

    ValueTable() = default;
};

using Value =
    std::variant<ValueNil, ValueBoolean, ValueNumber, ValueString, ValueFunction, ValueTable>;

#endif
