/* 
 * File:   lexan.h
 * Author: lubos
 *
 * Created on May 31, 2012, 10:11 AM
 */

#ifndef LEXAN_H
#define	LEXAN_H

/* lexan.h */

typedef enum {
    IDENT, NUMB, PLUS, MINUS, TIMES, DIVIDE,
    EQ, NEQ, LT, GT, LTE, GTE, LPAR, RPAR, ASSGN,
    COMMA, SEMICOLON,
    kwVAR, kwCONST, kwBEGIN, kwEND, kwIF, kwTHEN, kwELSE,
    kwWHILE, kwDO, kwWRITE,
    EOI
} LexSymbol;

enum {
    MaxLenIdent = 32
};

extern LexSymbol Symb;
extern char Ident[MaxLenIdent]; /* atribut symbolu IDENT */
extern int Cislo; /* atribut symbolu NUMB */

void CtiSymb(void);
void InitLexan(char*);
void Chyba(const char*);
void ChybaSrovnani(LexSymbol);

#endif	/* LEXAN_H */

