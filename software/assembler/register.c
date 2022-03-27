
#include "asm.h"


struct reg_t {
    char *regName;
    int bits;
};

struct reg_t registers[MAX_REGISTERS] = { { 0 } };



struct reg_t *AddRegister(char *r, int b)
{
    for (int i = 0; i < MAX_REGISTERS; i ++)
    {
        if (registers[i].regName == 0) {
            registers[i].regName = r;
            registers[i].bits = b;
            return &registers[i];
        }

        if (strcmp(r, registers[i].regName) == 0) {
            Error("Register %s already defined as %d bits wide", sourceFile, yylineno, 0, 0, r, b);
            return &registers[i];
        }
    }

    Error("Too many registers", sourceFile, yylineno, 0, 0);
    return 0;
}


void DumpRegisters(void)
{
    for (int i = 0; i < MAX_REGISTERS; i ++)
    {
        if (registers[i].regName == 0) return;

        Msg("Register %s is %d bytes wide\n", registers[i].regName, registers[i].bits);
    }
}

