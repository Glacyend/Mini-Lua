#ifndef __OPERATORS_H__
#define __OPERATORS_H__

enum class Operator {
    Or,
    And,
    LessThan,
    GreaterThan,
    LessEqual,
    GreaterEqual,
    NotEqual,
    Equal,
    Concat,
    Plus,
    Minus,
    Multiply,
    Divide,
    Mod,
    Not,
    Length,
    Negative,
    Power,
};

#ifdef DEBUG_MODE

const char* operator_to_string(Operator ope);

#endif

#endif
