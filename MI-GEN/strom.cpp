/* strom.cpp */
#include "strom.h"
#include "gener.h"
#include "util.h"

#define DEBUG_OPT

extern FILE* debugPrint;

// konstruktory a destruktory

Var::Var(int a, bool rv) {
    addr = a;
    rvalue = rv;
}

Numb::Numb(int v) {
    value = v;
}

int Var::getAddr() const {
    return addr;
}

bool Var::isRValue() const {
    return rvalue;
}

int Numb::getValue() const {
    return value;
}

Bop::Bop(Operator o, Expr *l, Expr *r) {
    op = o;
    left = l;
    right = r;
}

Bop::~Bop() {
    SAFE_DELETE(left);
    SAFE_DELETE(right);
}

UnMinus::UnMinus(Expr *e) {
    expr = e;
}

UnMinus::~UnMinus() {
    SAFE_DELETE(expr);
}

Assign::Assign(Var *v, Expr *e) {
    var = v;
    expr = e;
}

Assign::~Assign() {
    SAFE_DELETE(var);
//  SAFE_DELETE(expr);
}

Write::Write(Expr *e) {
    expr = e;
}

Write::~Write() {
    SAFE_DELETE(expr);
}

If::If(Expr *c, Statm *ts, Statm *es) {
    cond = c;
    thenstm = ts;
    elsestm = es;
}

If::~If() {
    SAFE_DELETE(cond);
    SAFE_DELETE(thenstm);
    SAFE_DELETE(elsestm);

}

While::While(Expr *c, Statm *b) {
    cond = c;
    body = b;
}

While::~While() {
    SAFE_DELETE(cond);
//  SAFE_DELETE(body);
}

StatmList::StatmList() {
    statm = NULL;
    next = NULL;
}

void StatmList::setStm(Statm* stm) {
    statm = stm;
}

Statm* StatmList::getStm() const {
    return statm;
}

StatmList* StatmList::getNext() const {
    return next;
}

void StatmList::setNext(StatmList* nxt) {
    next = nxt;
}

StatmList::StatmList(Statm *s, StatmList *n) {
    statm = s;
    next = n;
}

StatmList::~StatmList() {
    SAFE_DELETE(statm);
    SAFE_DELETE(next);
}

Prog::Prog(StatmList *s) {
    stm = s;
}

Prog::~Prog() {
    SAFE_DELETE(stm);
}

void Numb::Print() const {
    fprintf(debugPrint, "[ Const(%d) ]", value);
}

void Var::Print() const {
    fprintf(debugPrint, "[ Var(%d) ]", addr);
}

void Assign::Print() const {
    fprintf(debugPrint, "[ Assign ");
    var->Print();
    expr->Print();
    fprintf(debugPrint, " ]\n");
}

void UnMinus::Print() const {
    fprintf(debugPrint, "[ UnMinus ");
    expr->Print();
    fprintf(debugPrint, " ]\n");
}

void Bop::Print() const {
    fprintf(debugPrint, "[ Bop(%s)", toOpString(op));
    left->Print();
    right->Print();
    fprintf(debugPrint, " ]\n");
}

void Prog::Print() const {
    fprintf(debugPrint, "[ Prog ");
    stm->Print();
    fprintf(debugPrint, " ]\n");
}

void StatmList::Print() const {
    fprintf(debugPrint, "[ StmList ");
    const StatmList *s = this;
    do {
        s->statm->Print();
        s = s->next;
    } while (s);
    fprintf(debugPrint, " ]\n");
}

void While::Print() const {
    fprintf(debugPrint, "[ While ");
    cond->Print();
    body->Print();
    fprintf(debugPrint, " ]\n");
}

void If::Print() const {
    fprintf(debugPrint, "[ If ");
    cond->Print();
    thenstm->Print();
    elsestm->Print();
    fprintf(debugPrint, " ]\n");
}

void Write::Print() const {
    fprintf(debugPrint, "[ Write ");
    expr->Print();
    fprintf(debugPrint, " ]\n");
}

/*
 * staticka funkce pro overeni, zda se jedna o jednu
 * a tu samou konstantu
 */
bool Numb::areTheSameNumbers(Expr* e1, Expr* e2) {
    Numb* n1 = dynamic_cast<Numb*> (e1);
    Numb* n2 = dynamic_cast<Numb*> (e2);

    if (n1 == NULL || n2 == NULL) {
        return false;
    }

    if (n1->getValue() != n2->getValue()) {
        return false;
    }
    return true; // jsou stejne
}

/*
 * staticka funkce pro overeni, zda se jedna
 * o jednu a tu samou promennou
 */
bool Var::areTheSameVars(Expr* e1, Expr* e2) {
    Var* var1 = dynamic_cast<Var*> (e1);
    Var* var2 = dynamic_cast<Var*> (e2);

    if (var1 == NULL || var2 == NULL) {
        return false;
    }

    if (var1->getAddr() != var2->getAddr()) {
        return false;
    }
    return true; //jsou stejne
}

/*
 * staticka funkce pro overeni, zda jsou dva Bop podvyrazy 
 * semanticky ekvivalentni
 */
bool Bop::areBopTreeTheSame(Expr* e1, Expr* e2) {
    Bop* b1 = dynamic_cast<Bop*> (e1);
    Bop* b2 = dynamic_cast<Bop*> (e2);

    if (b1 == NULL || b2 == NULL) {
        return false;
    }

    if (b2->op != b2->op) {
        return false;
    }

    if (Var::areTheSameVars(b1->left, b2->left)) {
        if (Var::areTheSameVars(b1->right, b2->right)) {
            return true;
        } else if (Numb::areTheSameNumbers(b1->right, b2->right)) {
            return true;
        } else {
            return false;
        }
    }

    if (Var::areTheSameVars(b1->left, b2->right)) {
        if (Var::areTheSameVars(b1->right, b2->left)) {
            return true;
        } else if (Numb::areTheSameNumbers(b1->right, b2->left)) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void Bop::getExpressionBops(std::vector<Bop*>& bops) {
    left->getExpressionBops(bops);
    bops.push_back(this);
    right->getExpressionBops(bops);
}


// definice metody Optimize

Node *Bop::Optimize() {
    left = (Expr*) (left->Optimize());
    right = (Expr*) (right->Optimize());
    Numb *l = dynamic_cast<Numb*> (left);
    Numb *r = dynamic_cast<Numb*> (right);
    if (!l || !r) {
        return this;
    }
    int res;
    int leftval = l->getValue();
    int rightval = r->getValue();
    switch (op) {
        case Plus:
            res = leftval + rightval;
            break;
        case Minus:
            res = leftval - rightval;
            break;
        case Times:
            res = leftval * rightval;
            break;
        case Divide:
            res = leftval / rightval;
            break;
        case Eq:
            res = leftval == rightval;
            break;
        case NotEq:
            res = leftval != rightval;
            break;
        case Less:
            res = leftval < rightval;
            break;
        case Greater:
            res = leftval > rightval;
            break;
        case LessOrEq:
            res = leftval <= rightval;
            break;
        case GreaterOrEq:
            res = leftval >= rightval;
            break;
    }
    delete this;
    return new Numb(res);
}

Node *UnMinus::Optimize() {
    expr = (Expr*) expr->Optimize();
    Numb *e = dynamic_cast<Numb*> (expr);
    if (!e) {
        return this;
    }
    e = new Numb(-e->getValue());
    delete this;
    return e;
}

/*
 * funkce, která vrací mapu Binárních operací jako kličí k hodnotám, 
 * které tvorí seznam, ekvivalentních binárních operací
 */
std::map<Bop*, std::vector<Bop*> > Bop::findEquivalentBops(std::vector<Bop*>& bops) {
    std::map<Bop *, std::vector<Bop *> > same;    

    for (unsigned int i = 0; i < bops.size(); ++i) {
        for (unsigned int j = i + 1; j < bops.size(); ++j) {
            if (Bop::areBopTreeTheSame(bops[i], bops[j]) == true) {
                same[bops[i]].push_back(bops[j]);
            }
        }
        std::vector<Bop *> vec = same[bops[i]];
        if (vec.size() > 0) {
            same[bops[i]].push_back(bops[i]);
        }
    }
    return same;
}

/*
 * funkce vraci Bop, ktery bude nejvyhodnejsi optimalizovat, 
 * nebo prvni, ktery je nejlesi s nejakym daslim
 * 
  std::map<Bop *, std::vector<Bop *> >::iterator max;
  Bop *maxBop = NULL;
  int maxcount = 0;
  for (duplit=dupl.begin(); duplit!=dupl.end(); ++duplit) {
    if (duplit->second.size() > maxcount) {
      maxcount = (*duplit).second.size();
      max = duplit;
      maxBop = duplit->first;
    }
  }
 */
Bop* Bop::getWhereOptimize(std::map<Bop*, std::vector<Bop*> >& same) {
    std::map<Bop*, std::vector<Bop*> >::iterator itSame;
    unsigned int count = 0;
    Bop *ret = NULL;
       
    for (itSame = same.begin(); itSame != same.end(); ++itSame) {
        if (itSame->second.size() > count) {
            count = (*itSame).second.size();            
            ret = itSame->first;
        }
    }
    return ret;    
}

/*
 * funkce pro samotnz replace za Var
 */
void Bop::replaceExpression(std::vector<Bop*>& bops) {
    this->left->replaceExpression(bops);
    this->right->replaceExpression(bops);

    std::vector<Bop *>::iterator ii;
    for (ii = bops.begin(); ii != bops.end(); ++ii) {
        if (this->left == *ii) {
            delete this->left;
            this->left = new Var(0, true);
        }
        if (this->right == *ii) {
            delete this->right;
            this->right = new Var(0, true);
        }
    }
}

/*
 * Optimalizace podvyrazu
 */
Node *Assign::Optimize() {
    expr = (Var*) (expr->Optimize());
    // nejprve ve vyrazu najdeme vsechny podstromy
    // s binarni operaci, na nez si ulozime pointery
    std::vector<Bop*> bops;
    std::vector<Bop*>::iterator itBops;
    expr->getExpressionBops(bops);
#ifdef DEBUG_OPT
    if(bops.size() > 0) {
        printMessage("Optimalizace podvyrazu");
    }
    for (itBops = bops.begin(); itBops != bops.end(); ++itBops) {
        (*itBops)->Print();
    }
#endif
    //najdeme mezi nimi ekvivalenty a vratime svazane v mape
    std::map<Bop*, std::vector<Bop*> >::iterator itSame;
    std::map<Bop*, std::vector<Bop*> > same = Bop::findEquivalentBops(bops);
#ifdef DEBUG_OPT
    for (itSame = same.begin(); itSame != same.end(); ++itSame) {
        itSame->first->Print();
    }
#endif
    //zjistime kde mistech je nejlepsi optimalizvat v ramci vyrazu
    Bop* bestBop = Bop::getWhereOptimize(same);

    if (bestBop != NULL) {
        // zde jsou podvyrazy k prepsani
        std::vector<Bop *> toReplace = same.find(bestBop)->second;
#ifdef DEBUG_OPT
        for (itBops = toReplace.begin(); itBops != toReplace.end(); ++itBops) {
            (*itBops)->Print();
        }
#endif
        if (toReplace.size() > 0) {
            // vytvorime nove prirazeni a nahradime vyraz
//            Assign *newAssign = new Assign(new Var(0, false), new Numb(3));
            toReplace.pop_back();
            // vytvori se novy Stm blok a nahradi stavajici
            StatmList *tmpList = new StatmList();
            tmpList->setStm(this);
            StatmList *newList = new StatmList(new Empty(), tmpList);
            expr->replaceExpression(toReplace);
            return newList;
        }
    }
    //pokud se nepodarilo nalezt kandidata na optimalizaci,
    //tak se zadna optimalizace nedela(na this se nic nemeni)
    return this;
}

Node *Write::Optimize() {
    expr = (Expr*) (expr->Optimize());
    return this;
}

Node *If::Optimize() {
    cond = (Expr*) (cond->Optimize());
    thenstm = (Statm*) (thenstm->Optimize());
    if (elsestm) {
        elsestm = (Statm*) (elsestm->Optimize());
    }
    Numb *c = dynamic_cast<Numb*> (cond);
    if (!c) {
        return this;
    }
    Node *res;
    if (c->getValue()) {
        res = thenstm;
        thenstm = 0;
    } else {
        res = elsestm;
        elsestm = 0;
    }
    delete this;
    return res;
}

/*
 * funkce vraci mapu s pointry na prikazy, ktere jsou invarianty v cyklu
 */
std::map<int, StatmList *> While::getInvariants(StatmList* whileBody) {
    std::map<int, StatmList *> toReplace;
    Assign* assign = NULL;

    //projdeme vsechny prikazy v tele
    while (whileBody != NULL) {
        assign = dynamic_cast<Assign *> (whileBody->getStm());
        //pokud je prikaz prirazeni, muze byt invariantem
        if (assign != NULL) {
#ifdef DEBUG_OPT                                
            printf("Nalezeno prizazeni\n");
#endif                
            // pokdu je na prave strane prirazeni konstanta => mame invariant
            if (dynamic_cast<Numb *> (assign->getExpr()) != NULL) {
#ifdef DEBUG_OPT                   
                printf("Nalezen invariant => prirazeni konstanty do promenne %i\n", assign->getVar()->getAddr());
#endif                    
                toReplace[assign->getVar()->getAddr()] = whileBody;
            }
        }
        whileBody = whileBody->getNext();
    }
    return toReplace;
}

/*
 * funkce ktera nahradi cely while novym statm listem, kde jako prvni zapoji
 * invarianty a pak zbytek tela cyklu
 */
StatmList* While::replaceInvariants(std::map<int, StatmList*> toReplace, StatmList* whileBody) {
    StatmList *newWhileBlock = new StatmList();
    std::map<int, StatmList *>::iterator ii;
    // projdeme celou mapu invariantu
    for (ii = toReplace.begin(); ii != toReplace.end(); ++ii) {
        StatmList* remove = ii->second;
        StatmList* start = whileBody;

        if (newWhileBlock->getStm() == NULL) {
            newWhileBlock->setStm(this);
        }
        // pokud je invariant jako prvni
        if (start == remove) {
            StatmList *tmpStmList = new StatmList(start->getStm(), newWhileBlock);
            start->setStm(new Empty());
            newWhileBlock = tmpStmList;
        } else {
            while (start->getNext() != remove) {
                start = start->getNext();
            }
            //minula pozice se musi preskocit
            //TODO leak            
            if (start->getNext() != NULL) {
                start->setNext(start->getNext()->getNext());
            }
            start = new StatmList(remove, newWhileBlock);
            newWhileBlock = start;
        }
    }
    // toto by se stat nemelo, ale je dobre to zkontrolovat
    if (newWhileBlock->getStm() == NULL || newWhileBlock->getNext() == NULL) {
        SAFE_DELETE(newWhileBlock);
        return NULL;
    }
    return newWhileBlock;
}

/*
 * optimalizace invarintu v cyklu
 */
Node *While::Optimize() {
    cond = (Expr*) (cond->Optimize());
    body = (Statm*) (body->Optimize());

    Numb *c = dynamic_cast<Numb*> (cond);
    if (c != NULL) {
        if (!c->getValue()) {
            delete this;
            return new Empty();
        }
    }
#ifdef DEBUG_OPT
    printMessage("While telo pred optimalizaci");
    this->Print();
#endif        
    //nejprve je potreba zjistit, zda telo je typu list prikazu
    StatmList *whileBody = dynamic_cast<StatmList *> (body);
    if (whileBody == NULL) {
        return this;
    }
#ifdef DEBUG_OPT        
    printf("While telo je typu StatmList\n");
#endif            
    //nehcame vratit mapu s invarianty
    std::map<int, StatmList* > replacelist = getInvariants(whileBody);
    if (replacelist.empty()) {
        return this;
    }
    //presuneme invarianty pred tento cyklus(kdyby nahodou byly potreba uvnitr)
    StatmList* newWhile = replaceInvariants(replacelist, whileBody);
    if (newWhile == NULL) {
        return this;
    }
#ifdef DEBUG_OPT                    
    printMessage("While po uplatneni otimalizace");
    newWhile->Print();
#endif                  
    return newWhile;
}

Node *StatmList::Optimize() {
    StatmList *s = this;
    do {
        s->statm = (Statm*) (s->statm->Optimize());
        s = s->next;
    } while (s);
    return this;
}

Node *Prog::Optimize() {
    stm = (StatmList*) (stm->Optimize());
    return this;
}

/*
 * pri prekladu listu AST s promennou
 * udela akorat load
 */
void Var::Translate() {
    if (traceCode) {
        emitComment(">> Ident");
    }
    emitRM("LD", acc1, addr, gp, "load id value");
    if (traceCode) {
        emitComment("<< Ident");
    }
}

/*
 * pri prekladu listu AST s konstantou
 * je potreba udelat LDC(load konstatny)
 */
void Numb::Translate() {
    if (traceCode) {
        emitComment(">> Const");
    }
    emitRM("LDC", acc1, value, 0, "load const");
    if (traceCode) {
        emitComment("<< Const");
    }
}

/*
 * funkce pro preklad binarni operace
 * <, >, ==, !=...
 */
void Bop::Translate() {
    if (traceCode) {
        emitComment(">> Bop");
    }
    // generuje se kod leve strany(operandu)
    left->Translate();
    // store leveho operandu od acc
    emitRM("ST", acc1, tmpOffset--, mp, "Bop: push left");

    // generuje se kod prave strany(operandu)
    right->Translate();

    //ulozena leva strana se znovu loadne
    emitRM("LD", acc2, ++tmpOffset, mp, "Bop: load left");

    switch (op) {
        case Plus: //u kazde aritmeticke operace se provede nad obema acc
            emitRO("ADD", acc1, acc2, acc1, "Bop: +");
            break;
        case Minus:
            emitRO("SUB", acc1, acc2, acc1, "Bop: -");
            break;
        case Times:
            emitRO("MUL", acc1, acc2, acc1, "Bop: *");
            break;
        case Divide:
            emitRO("DIV", acc1, acc2, acc1, "Bop: /");
            break;
        case Less: //zde se akorat navic resi, kam se kdy byde skakat
            emitRO("SUB", acc1, acc2, acc1, "Bop: <");
            emitRM("JLT", acc1, 2, pc, "branch if lower than");
            emitRM("LDC", acc1, 0, acc1, "if not");
            emitRM("LDA", pc, 1, pc, "jump to end");
            emitRM("LDC", acc1, 1, acc1, "if true");
            break;
        case Eq:
            emitRO("SUB", acc1, acc2, acc1, "Bop: ==");
            emitRM("JEQ", acc1, 2, pc, "branch if equal");
            emitRM("LDC", acc1, 0, acc1, "if not");
            emitRM("LDA", pc, 1, pc, "jump to end");
            emitRM("LDC", acc1, 1, acc1, "if true");
            break;
        case NotEq:
            emitRO("SUB", acc1, acc2, acc1, "Bop: !=");
            emitRM("JNE", acc1, 2, pc, "branch if not equal");
            emitRM("LDC", acc1, 0, acc1, "if not");
            emitRM("LDA", pc, 1, pc, "jump to end");
            emitRM("LDC", acc1, 1, acc1, "if true");
            break;
        case Greater:
            emitRO("SUB", acc1, acc2, acc1, "Bop: >");
            emitRM("JGT", acc1, 2, pc, "branch if greater");
            emitRM("LDC", acc1, 0, acc1, "if not");
            emitRM("LDA", pc, 1, pc, "jump to end");
            emitRM("LDC", acc1, 1, acc1, "if true");
            break;
        case LessOrEq:
            emitRO("SUB", acc1, acc2, acc1, "Bop: !=");
            emitRM("JLE", acc1, 2, pc, "branch if lower or equal");
            emitRM("LDC", acc1, 0, acc1, "if not");
            emitRM("LDA", pc, 1, pc, "jump to end");
            emitRM("LDC", acc1, 1, acc1, "if true");
            break;
        case GreaterOrEq:
            emitRO("SUB", acc1, acc2, acc1, "Bop: !=");
            emitRM("JGE", acc1, 2, pc, "branch if greater or equal");
            emitRM("LDC", acc1, 0, acc1, "if not");
            emitRM("LDA", pc, 1, pc, "jump to end");
            emitRM("LDC", acc1, 1, acc1, "if true");
            break;
        default:
            emitComment("Hodne velka chyba -> Bop switch.");
            break;
    }
    if (traceCode) {
        emitComment("<< Bop");
    }
}

/*
 * unarni minus vlozi do registru nulu
 * a od ni odecte hodnotu
 */
void UnMinus::Translate() {
    if (traceCode) {
        emitComment(">> UnMinus");
    }
    expr->Translate();
    emitRM("LDC", acc2, 0, 0, "UnMinus: load const 0");
    emitRO("SUB", acc1, acc2, acc1, "UnMinus: value = 0 - value");
    if (traceCode) {
        emitComment("<< UnMinus");
    }
}

/*
 * nejdrive se generuje kod pro vyraz 
 * vpravo, pak se priradi(udela store)
 */
void Assign::Translate() {
    if (traceCode) {
        emitComment(">> assign");
    }
    expr->Translate();
    emitRM("ST", acc1, var->getAddr(), gp, "assign: store value");
    if (traceCode) {
        emitComment("<< assign");
    }
}

/*
 * funkce generuje prikaz pro vystup 
 * z nejakeho R 
 */
void Write::Translate() {
    // vypisuje se vyraz, ktery je potreba nejdrive prelozit(spocitat)
    expr->Translate();
    emitRO("OUT", acc1, 0, 0, "write ac");
}

/*
 * funkce, ktera generuje kod pro podminky
 */
void If::Translate() {
    if (traceCode) {
        emitComment(">> if");
    }
    //generuje se kod podminky
    cond->Translate();
    //ulozi se lokace v kodu na dalsi instrukci
    int savedLoc1 = emitSkip(1);
    emitComment("if: jump to else belongs here");

    //generovani splnene vetve
    thenstm->Translate();
    int savedLoc2 = emitSkip(1);
    emitComment("if: jump to end belongs here");
    int currentLoc = emitSkip(0);
    emitBackup(savedLoc1);
    emitRM_Abs("JEQ", acc1, currentLoc, "if: jmp to else");
    emitRestore();

    //else je nepovinny 
    if (elsestm) {
        // generuje se nesplnena vetev
        elsestm->Translate();
        currentLoc = emitSkip(0);
        //skoci se zpet
        emitBackup(savedLoc2);
        //a do pc se load ne spravna hodnota
        emitRM_Abs("LDA", pc, currentLoc, "jmp to end");
        emitRestore();
    } else {
        emitBackup(savedLoc1);
        emitRestore();
    }

    if (traceCode) {
        emitComment("<< If");
    }
}

/*
 * funkce, generuje cyklu
 */
void While::Translate() {
    if (traceCode) {
        emitComment(">> while");
    }
    int start = emitSkip(0);
    cond->Translate();
    int ifjump = emitSkip(1);
    body->Translate();
    emitRM_Abs("LDA", pc, start, "jmp back to cond");

    int end = emitSkip(0);
    emitBackup(ifjump);
    emitRM_Abs("JEQ", acc1, end, "jmp to end");

    // restore adresy po tele cyklu -> end
    emitRestore();
    if (traceCode) {
        emitComment("<< While");
    }
}

/*
 * prochazi cely spojovy seznam prikazu
 */
void StatmList::Translate() {
    StatmList *s = this;
    do {
        if (s->statm != NULL) {
            s->statm->Translate();
        }
        s = s->next;
    } while (s);
}

/*
 * vstupni bod prekladu
 */
void Prog::Translate() {

    fprintf(outputCode, "* TinyMachine Code:\n");

    emitRM("LD", mp, 0, acc1, "load memsize from 0");
    emitRM("ST", acc1, 0, acc1, "clear location 0");
    emitComment("End of standard prelude.");

    // generovani z AST od korene stm(zacatatku spojaku)
    stm->Translate();

    emitComment("End of execution.");
    emitRO("HALT", 0, 0, 0, "");
}

Expr *VarOrConst(char *id) {
    int v;
    DruhId druh = idPromKonst(id, &v);
    switch (druh) {
        case IdProm:
            return new Var(v, true);
        case IdKonst:
            return new Numb(v);
        default:
            std::cout << "Chyba: VarOrConst" << std::endl;
            return NULL;
    }
}


