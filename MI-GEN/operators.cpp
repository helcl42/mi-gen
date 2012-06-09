#include "operators.h"

const char* toOpString(Operator op) {
    switch(op) {
        case Plus:
            return "+";            
        case Minus:
            return "-";           
        case Times:
            return "*";            
        case Divide:
            return "/";            
        case Less:
            return "<";            
        case Greater:
            return ">";            
        case LessOrEq:
            return "<=";            
        case GreaterOrEq:
            return ">=";            
        case Eq:
            return "==";
        case NotEq:
            return "!=";
        default:
            return "UNKNOWN OP";
    }
}

