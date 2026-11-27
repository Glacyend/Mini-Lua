#ifndef __VALUE_H__
#define __VALUE_H__

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

using Value = std::variant<ValueNil, ValueBoolean, ValueNumber>;

#endif
