

#include "asm.h"
#include <stddef.h>
#include <stdint.h>



struct OpCode_t {
    char *opcodeDef;
    char *byteStream;
};


struct OpCode_t opcodeList[MAX_OPCODES] = { { 0 } };


void AddOpcode(char *def, char *bytes)
{
    for (int i = 0; i < MAX_OPCODES; i ++) {
        if (opcodeList[i].opcodeDef == 0 && opcodeList[i].byteStream == 0) {
            opcodeList[i].opcodeDef = def;
            opcodeList[i].byteStream = bytes;

            return;
        }
    }

    Fatal("Too many opcode definitions", sourceFile, yylineno, 0, 0);
}


void DumpOpcodes(void)
{
    for (int i = 0; i < MAX_OPCODES; i ++) {
        if (opcodeList[i].opcodeDef == 0 || opcodeList[i].byteStream == 0) return;
        Msg("Opcode definition `%s` outputs bytes `%s`", opcodeList[i].opcodeDef, opcodeList[i].byteStream);
    }
}

char *NormalizeInstruction(char *line)
{
    int len = strlen(line) + 1;
    char *rv = malloc(len);
    char *ch = line;
    char *out = rv;

    memset(rv, 0, len);

    // -- start by skipping all leading blanks
    while (*ch == ' ' || *ch == '\t') ch ++;

    // -- now move in the mnemonic with a separating space
    while (*ch != ' ' && *ch != '\t') {
        if (*ch == 0) return rv;
        *out ++ = *ch ++;
    }

    *out ++ = ' ';

    // -- the rest just needs to have any blanks dropped until the end of the input line
    while (*ch) {
        if (*ch != ' ' && *ch != '\t') {
            *out ++ = *ch;
        }

        ch ++;
    }

    if (*(-- out) == ' ') *out = 0;

    return rv;
}


int ParseNumber(char *_s, uint64_t *val, int base)
{
    // -- TODO: add some protections into this function
    char *s = _s;
    *val = 0;

    while (*s) {
        *s = tolower(*s);

        int d;

        if (*s >= '0' && *s <= '9') {
            d = *s - '0';
        } else if (*s >= 'a' && *s <= 'f') {
            d = *s - 'a' + 10;
        } else if (*s >= 'A' && *s <= 'F') {
            d = *s - 'A' + 10;
        } else {
            break;
        }

        *val = *val * base + d;
        s ++;
    }

    return s - _s;
}

int ParseDecimal(char *s, uint64_t *v) { return ParseNumber(s, v, 10); }
int ParseBinary(char *s, uint64_t *v) { return ParseNumber(s + 2, v, 2) + 2; }
int ParseOctal(char *s, uint64_t *v) { return ParseNumber(s, v, 8); }
int ParseHex(char *s, uint64_t *v) { return ParseNumber(s + 2, v, 16) + 2; }


struct OpCode_t *FindInstruction(char *instr, uint64_t *val)
{
    for (int i = 0; i < MAX_OPCODES; i ++) {
        if (!opcodeList[i].opcodeDef) break;

        char *mLoc = strstr(opcodeList[i].opcodeDef, "$(");


        if (mLoc == 0) {
            if (strcmp(instr, opcodeList[i].opcodeDef) == 0) {
                return &opcodeList[i];
            }
        } else {
            // -- get the position of the possible number in the instruction
            size_t len = mLoc - opcodeList[i].opcodeDef;
            if (strncmp(instr, opcodeList[i].opcodeDef, len) != 0) continue;

            // -- a number will start with, well, a digit; anything else is not a number
            if (instr[len] < '0' || instr[len] > '9') {
                continue;
            }

            // -- well, we know we have a number to convert; what kind is it?
            int numDigits;

            if (instr[len] != '0') {
                numDigits = ParseDecimal(instr + len, val);
            } else if (instr[len + 1] == 'b') {
                numDigits = ParseBinary(instr + len, val);
            } else if (instr[len + 1] == 'x') {
                numDigits = ParseHex(instr + len, val);
            } else {
                numDigits = ParseOctal(instr + len, val);
            }

            if (strncmp(mLoc, "$(8)", 4) == 0) {
                if (strcmp(instr + len + numDigits, opcodeList[i].opcodeDef + len + 4) == 0) {
                    if ((*val & 0xff) == *val) {
                        return &opcodeList[i];
                    }
                } else continue;
            }

            if (strncmp(mLoc, "$(16)", 5) == 0) {
                if (strcmp(instr + len + numDigits, opcodeList[i].opcodeDef + len + 5) == 0) {
                    if ((*val & 0xffff) == *val) {
                        return &opcodeList[i];
                    }
                } else continue;
            }
        }
    }

    Error("Unknown instruction; %s", sourceFile, yylineno, 0, 0, instr);
    return 0;
}


void ParseInstruction(char *line)
{
    uint64_t val = 0;

    struct OpCode_t *instr = FindInstruction(line, &val);
    EmitInstruction(instr->byteStream, val);
}



