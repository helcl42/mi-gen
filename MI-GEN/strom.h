/* 
 * File:   strom.h
 * Author: lubos
 *
 * Created on May 31, 2012, 10:13 AM
 */

#ifndef STROM_H
#define	STROM_H

#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

#include "tabsym.h"
#include "util.h"
#include "operators.h"

class Bop;

class Node {
public:

    virtual ~Node() {
    }

    virtual Node *Optimize() {
        return this;
    }
    virtual void Translate() = 0;
    virtual void Print() const = 0;
};

class Expr : public Node {
public:

    virtual void getExpressionBops(std::vector<Bop*>& bops) {
    }

    virtual void replaceExpression(std::vector<Bop*>& bops) {
    }
};

class Statm : public Node {
};

class Var : public Expr {
protected:
    int addr;
    bool rvalue;

public:
    Var(int, bool);
    virtual void Translate();
    virtual void Print() const;

    virtual void getExpressionBops(std::vector<Bop*>& bops) {
    };

    virtual void replaceExpression(std::vector<Bop*>& bops) {
    }

    int getAddr() const;
    bool isRValue() const;
    static bool areTheSameVars(Expr* e1, Expr* e2);
};

class Numb : public Expr {
protected:
    int value;

public:
    Numb(int);

    ~Numb() {
    }

    virtual void Translate();
    virtual void Print() const;

    virtual void getExpressionBops(std::vector<Bop*>& bops) {
    }

    virtual void replaceExpression(std::vector<Bop*>& bops) {
    }

    int getValue() const;
    static bool areTheSameNumbers(Expr* e1, Expr* e2);
};

class Bop : public Expr {
protected:
    Operator op;
    Expr *left;
    Expr *right;

public:
    Bop(Operator, Expr*, Expr*);
    virtual ~Bop();

    virtual Node *Optimize();
    virtual void Translate();
    virtual void Print() const;
    virtual void getExpressionBops(std::vector<Bop*>& bops);
    virtual void replaceExpression(std::vector<Bop*>& bops);

    static bool areBopTreeTheSame(Expr* e1, Expr* e2);
    static std::map<Bop*, std::vector<Bop*> > findEquivalentBops(std::vector<Bop*>& bops);
    static Bop* getWhereOptimize(std::map<Bop*, std::vector<Bop*> >& same);
};

class UnMinus : public Expr {
protected:
    Expr *expr;

public:
    UnMinus(Expr *e);
    virtual ~UnMinus();

    virtual Node *Optimize();
    virtual void Translate();
    virtual void Print() const;

    virtual void getExpressionBops(std::vector<Bop*>& bops) {
    }

    virtual void replaceExpression(std::vector<Bop*>& bops) {
    }
};

class Assign : public Statm {
protected:
    Var *var;
    Expr *expr;

public:
    Assign(Var*, Expr*);
    virtual ~Assign();

    virtual Node *Optimize();
    virtual void Translate();
    virtual void Print() const;

    Expr* getExpr() const {
        return expr;
    }

    Var* getVar() const {
        return var;
    }
};

class Write : public Statm {
protected:
    Expr *expr;

public:
    Write(Expr*);
    virtual ~Write();
    virtual Node *Optimize();
    virtual void Translate();
    virtual void Print() const;
};

class If : public Statm {
protected:
    Expr *cond;
    Statm *thenstm;
    Statm *elsestm;

public:
    If(Expr*, Statm*, Statm*);
    virtual ~If();
    virtual Node *Optimize();
    virtual void Translate();
    virtual void Print() const;
    //virtual void getExpressionVariables(std::map<int, int>& vars)       {}
};

class StatmList;

class While : public Statm {
protected:
    Expr *cond;
    Statm *body;

public:
    While(Expr*, Statm*);
    virtual ~While();
    virtual Node *Optimize();
    virtual void Translate();
    virtual void Print() const;

    std::map<int, StatmList *> getInvariants(StatmList* whileBody);
    StatmList* replaceInvariants(std::map<int, StatmList *> toReplace, StatmList* body);
};

class StatmList : public Statm {
protected:
    Statm *statm;
    StatmList *next;

public:
    StatmList();
    StatmList(Statm*, StatmList*);

    virtual ~StatmList();
    virtual Node *Optimize();
    virtual void Translate();
    virtual void Print() const;

    void setStm(Statm* stm);
    Statm* getStm() const;

    StatmList* getNext() const;
    void setNext(StatmList* next);
};

class Empty : public Statm {
public:

    virtual void Translate() {
    }

    virtual void Print() const {
    }
};

class Prog : public Node {
protected:
    StatmList *stm;
    static int maxVarAddr;
    static bool optimized;

public:
    Prog(StatmList*);
    virtual ~Prog();
    virtual Node *Optimize();
    virtual void Translate();
    virtual void Print() const;
    
    static void setMaxVarAddr(int max);
    static void setOptimized(bool is);
    static int  getMaxVarAddr();
    static bool isOptimized();
};


Expr *VarOrConst(char*);


#endif	/* STROM_H */

