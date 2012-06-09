/* 
 * File:   gener.h
 * Author: lubos
 *
 * Created on May 10, 2012, 2:09 AM
 */

#ifndef GENER_H
#define	GENER_H


/*
 * enable traceCoding
 */
extern bool traceCode;

/*
 * output to file/stdout
 */
extern FILE *outputCode;

/*
 * program counter
 */
static const int pc = 7;

/*
 *  mp = "memory pointer" points to top of memory (tmp store) 
 */
static const int mp = 6;

/*
 *  gp = "global pointer" points to bottom of memory for (global) variable storage 
 */
static const int gp = 5;

/*
 *  accumulator 1
 */
static const int acc1 = 0;

/*
 * accumululator 2
 */ 
static const int acc2 = 1;

/* 
 * TM location number for current instruction emission 
 */
static int emitLoc = 0;

/* 
 * Highest TM location emitted so far
 * For use in conjunction with emitSkip,
 * emitBackup, and emitRestore 
 */
static int highEmitLoc = 0;

/*
 *  offset in memory -> like stack poiter 
 */
static int tmpOffset = 0;

/* 
 * Procedure emitComment prints a comment line 
 * with comment c in the code file 
 */
void emitComment(std::string c) {
    if (traceCode)
        fprintf(outputCode, "* %s\n", c.c_str());
}

/* 
 * Procedure emitRO emits a register-only
 * TM instruction
 * op = the opcode
 * r = target register
 * s = 1st source register
 * t = 2nd source register
 * c = a comment to be printed if TraceCode is TRUE 
 */
void emitRO(std::string op, int r, int s, int t, std::string comment) {
    fprintf(outputCode, "%3d:  %5s  %d,%d,%d ", emitLoc++, op.c_str(), r, s, t);
    if (traceCode)
        fprintf(outputCode, "\t%s", comment.c_str());
    fprintf(outputCode, "\n");
    if (highEmitLoc < emitLoc)
        highEmitLoc = emitLoc;
}

/* 
 * Procedure emitRM emits a register-to-memory
 * TM instruction
 * op = the opcode
 * r = target register
 * d = the offset
 * s = the base register
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitRM(std::string op, int r, int d, int s, std::string comment) {
    fprintf(outputCode, "%3d:  %5s  %d,%d(%d) ", emitLoc++, op.c_str(), r, d, s);
    if (traceCode)
        fprintf(outputCode, "\t%s", comment.c_str());
    fprintf(outputCode, "\n");
    if (highEmitLoc < emitLoc)
        highEmitLoc = emitLoc;
}

/* 
 * Function emitSkip skips "howMany" code
 * locations for later backpatch. It also
 * returns the current code position
 */
int emitSkip(int howMany) {
    int i = emitLoc;
    emitLoc += howMany;
    if (highEmitLoc < emitLoc)
        highEmitLoc = emitLoc;
    return i;
}

/* 
 * Procedure emitBackup backs up to 
 * loc = a previously skipped location
 */
void emitBackup(int loc) {
    if (loc > highEmitLoc)
        emitComment("BUG in emitBackup");
    emitLoc = loc;
}

/* 
 * Procedure emitRestore restores the current 
 * code position to the highest previously
 * unemitted position
 */
void emitRestore(void) {
    emitLoc = highEmitLoc;
}

/* 
 * Procedure emitRM_Abs converts an absolute reference 
 * to a pc-relative reference when emitting a
 * register-to-memory TM instruction
 * op = the opcode
 * r = target register
 * a = the absolute location in memory
 * c = a comment to be printed if TraceCode is TRUE
 */
void emitRM_Abs(std::string op, int r, int a, std::string comment) {
    std::fprintf(outputCode, "%3d:  %5s  %d,%d(%d) ", emitLoc, op.c_str(), r, a - (emitLoc + 1), pc);
    ++emitLoc;
    if (traceCode)
        fprintf(outputCode, "\t%s", comment.c_str());
    fprintf(outputCode, "\n");
    if (highEmitLoc < emitLoc)
        highEmitLoc = emitLoc;
}


#endif	/* GENER_H */

