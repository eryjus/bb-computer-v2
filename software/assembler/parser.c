//
// -- Compile commands (from project root, not this folder):
//
//    flex -iso software/assembler/lexer.c software/assembler/lexer.ll
//    clang -o software/assembler/asm software/assembler/*.c
//    ================================================================



#include "asm.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex (void);

int tok;
#define ADVANCE_TOKEN do { tok = yylex(); /* printf("%d\n", tok); */ } while (0)
#define CURRENT_TOKEN tok
#define MATCH(t) (tok == t)

#define UNIMPLEMENTED   do {                                                                                \
    fprintf(stderr, "Unimplemented token in parser (%s:%d): %d\n", __FILE__, __LINE__, CURRENT_TOKEN);      \
    exit(EXIT_FAILURE);                                                                                     \
} while (0)

#define RECOVERY        do {                                                                                \
        do {                                                                                                \
            ADVANCE_TOKEN;                                                                                  \
        } while (!MATCH(TOK_EOL));                                                                          \
        ADVANCE_TOKEN;                                                                                      \
    } while (0)



void OpenFile (const char *file);
char *sourceFile;



//
// -- Perform a register definition:
//    .register reg-name bits-wide\n
//    ------------------------------
void ParseRegisterDef(void) {
    if (!MATCH(TOK_ARCH_NAME)) {
        Error("Expected a register name", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    char *regName = yylval.name;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("Specify the number of bits wide the register is", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    AddRegister(regName, yylval.number);

    ADVANCE_TOKEN;

    if (!MATCH(TOK_EOL)) {
        Error("Too many tokens on line after register bit width: %d", sourceFile, yylineno, 0, 0, CURRENT_TOKEN);
        RECOVERY;
        return;
    }

    ADVANCE_TOKEN;
}


//
// -- Perform a memory definition:
//    .memory from-address to-address\n
//    ---------------------------------
void ParseMemoryDef(void) {
    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("Expected memory address range starting address", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    uint64_t fromAddr = yylval.number;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("Expected memory address range ending address", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    uint64_t toAddr = yylval.number;
    AddMemoryRange(fromAddr, toAddr);
    ADVANCE_TOKEN;

    if (!MATCH(TOK_EOL)) {
        Error("Too many tokens on line after memory address range: %d", sourceFile, yylineno, 0, 0, CURRENT_TOKEN);
        RECOVERY;
        return;
    }

    ADVANCE_TOKEN;
}


char *StandardizeOpcodeDef(void)
{
    int len = strlen(yylval.name) + 1;
    char *rv = malloc(len);
    char *ch = yylval.name;
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


char *StandardizeByteStream(void)
{
    int len = strlen(yylval.name) + 1;
    char *rv = malloc(len);
    char *ch = yylval.name;
    char *out = rv;

    memset(rv, 0, len);

    // -- start by skipping all leading blanks
    while (*ch == ' ' || *ch == '\t') ch ++;

    // -- now move in each byte with 1 space in between
    while (*ch) {
        while (*ch != ' ' && *ch != '\t') {
            if (*ch == 0) return rv;
            *out ++ = *ch ++;
        }

        while (*ch == ' ' || *ch == '\t') ch ++;

        *out ++ = ' ';
    }

    // -- back out the final space
    *(-- out) = 0;

    return rv;
}


//
// -- Perform an opcode definition:
//    -----------------------------
void ParseOpcodeDef(void)
{
    char *defn = StandardizeOpcodeDef();
    ADVANCE_TOKEN;

    if(!MATCH(TOK_OPCODE_DB)) {
        Error("Opcode definition requires a byte stream to emit", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    ADVANCE_TOKEN;

    char *bytes = StandardizeByteStream();
    AddOpcode(defn, bytes);
    ADVANCE_TOKEN;
}


void GetNewLocation(void)
{
    if (!MATCH(TOK_NUMBER)) {
        Error("`.org` location expected", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    SetPosition(yylval.number);
    ADVANCE_TOKEN;
}


void ParseFile(void)
{
    ADVANCE_TOKEN;

    while (CURRENT_TOKEN != 0) {
        switch (CURRENT_TOKEN) {
        case TOK_EOL:                   // -- handle an empty line (or one with just a comment)
            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_REGISTER:
            ADVANCE_TOKEN;
            ParseRegisterDef();
            break;

        case TOK_ARCH_MEMORY:
            ADVANCE_TOKEN;
            ParseMemoryDef();
            break;

        case TOK_ARCH_OPCODE:
            ADVANCE_TOKEN;
            ParseOpcodeDef();
            break;

        case TOK_ERROR:
            Error(yylval.errorMsg, sourceFile, yylineno, 0, 0);
            RECOVERY;
            break;

        case TOK_ORG:
            ADVANCE_TOKEN;
            GetNewLocation();
            break;

        case TOK_LABEL:
            NoteLabelLocation();
            ADVANCE_TOKEN;
            break;

        case TOK_INSTRUCTION:
            ParseInstruction(NormalizeInstruction(yylval.name));
            ADVANCE_TOKEN;
            break;

        default:
            UNIMPLEMENTED;
            RECOVERY;
        }
    }
}



int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i ++) {
        if (strcmp(argv[i], "-I") == 0) {
            i ++;
            for (int j = 0; j < MAX_SEARCH_PATH; j ++) {
                if (searchList[j] == 0) {
                    searchList[j] = argv[i];
                    break;
                }
            }

            fprintf(stderr, "Too many search paths\n");
            exit(EXIT_FAILURE);
        } else {
            sourceFile = argv[i];
        }
    }

    OpenFile(sourceFile);
    ParseFile();
    OutputResults();

    return EXIT_SUCCESS;
}

