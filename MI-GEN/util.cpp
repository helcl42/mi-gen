#include "util.h"

void printMessage(const char* msg, const char* data) {
    printf("\n------------------------------------------\n");
    if (!data)
        printf(" %s\n", msg);
    else
        printf(" %s %s\n", msg, data);
    printf("------------------------------------------\n");
}

