
/* syntakticky analyzator */

#include <stdio.h>
#include <string.h>

#include "util.h"
#include "operators.h"
#include "lexan.h"
#include "parser.h"
#include "strom.h"

FILE* outputCode;
FILE* debugPrint = stdout;
bool traceCode;
int maxVarAddr;
bool isOptimized;

void printUsage() {
    printf("\n-------------------------------------------------------------\n");
    printf("Usage: ./mila <input filepath> [-o] [-f outFilepath] [-d]\n");
    printf("-o\t\tuse this option for optimized code\n");
    printf("-f filepath\tuse this option to specify output file for code\n");
    printf("-d \t\t use this option for debug/comment output code\n");
    printf("-------------------------------------------------------------\n");
}

int main(int argc, char *argv[]) {
    char* filename;
    printMessage("Syntakticky analyzator:");
    if (argc == 1) {
        printMessage("Vstup z klavesnice, zadejte zdrojovy text");
        filename = NULL;
    } else {
        filename = argv[1];
        printMessage("Vstupni soubor: ", filename);
    }

    InitLexan(filename);
    CtiSymb();
    Prog *prog = Program();
    printMessage("AST pred optimalizaci v bracket notaci:");
    prog->Print();

    if (argc > 2) {
        if (strncmp(argv[2], "-opt", 4) == 0) {
            isOptimized = false;            
            maxVarAddr = getMaxIdentAddr() - 1;            
            while(!isOptimized) {
                prog = (Prog*) (prog->Optimize());                                          
            }
            printMessage("AST s optimalizaci v bracket notaci:");
            prog->Print();
        }
    }

    if (argc > 4) {
        if (strncmp(argv[3], "-f", 2) == 0) {
            outputCode = fopen(argv[4], "w");
            printMessage("Prelozeny program je v souboru:", argv[4]);
        } else {
            printUsage();
            return 0;
        }
    } else {
        printf("Prelozeny program:");
        outputCode = stdout;
    }

    if (argc > 5) {
        if (strncmp(argv[5], "-d", 2) == 0) {
            traceCode = true;
        } else {
            printUsage();
            return 0;
        }
    } else {
        traceCode = false;
    }

    prog->Translate();

    printf("Konec\n");
    SAFE_DELETE(prog);
    return 0;
}


