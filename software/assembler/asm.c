#include "asm.h"

char *searchList[MAX_SEARCH_PATH] = {
    "software/assembler/",
};


yylval_t yylval;


FILE *searchPath(char *filename)
{
    FILE *rv = fopen(filename, "r");

    if (rv) return rv;

    for (int i = 0; i < MAX_SEARCH_PATH; i ++) {
        char fullName[1024];

        if (!searchList[i]) return 0;

        sprintf(fullName, "%s%s", searchList[i], filename);

        rv = fopen(fullName, "r");

        if (rv) return rv;
    }

    return 0;
}




