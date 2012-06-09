/* 
 * File:   oprators.h
 * Author: lubos
 *
 * Created on May 31, 2012, 10:24 PM
 */

#ifndef OPRATORS_H
#define	OPRATORS_H

#include<iostream>

enum Operator {
    Plus, Minus, Times, Divide,
    Eq, NotEq, Less, Greater, LessOrEq, GreaterOrEq                       
};

const char* toOpString(Operator op);


#endif	/* OPRATORS_H */

