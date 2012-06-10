/* 
 * File:   tabsym.h
 * Author: lubos
 *
 * Created on May 31, 2012, 10:16 AM
 */

#ifndef TABSYM_H
#define	TABSYM_H

/* tabsym.h */

enum DruhId {
    Nedef, IdProm, IdKonst
};

void deklKonst(char*, int);
void deklProm(char*);
int adrProm(char*);
DruhId idPromKonst(char*, int*);
void printTabSym();
int getMaxIdentAddr();


#endif	/* TABSYM_H */

