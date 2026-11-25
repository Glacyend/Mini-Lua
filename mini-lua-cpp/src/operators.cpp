#include "operators.h"

#ifdef DEBUG_MODE

const char* operator_to_string(Operator ope) {
    switch (ope) {
        case Operator::Or: return "or";
        case Operator::And: return "and";
        case Operator::LessThan: return "<";
        case Operator::GreaterThan: return ">";
        case Operator::LessEqual: return "<=";
        case Operator::GreaterEqual: return ">=";
        case Operator::NotEqual: return "~=";
        case Operator::Equal: return "==";
        case Operator::Concat: return "..";
        case Operator::Plus: return "+";
        case Operator::Minus: return "-";
        case Operator::Multiply: return "*";
        case Operator::Divide: return "/";
        case Operator::Mod: return "%";
        case Operator::Not: return "not";
        case Operator::Length: return "#";
        case Operator::Negative: return "-";
        case Operator::Power: return "^";
    }
}

#endif
