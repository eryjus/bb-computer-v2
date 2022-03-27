
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>


#define MAX_REGISTERS   1024
#define MAX_SEARCH_PATH 50
#define MAX_OPERANDS    2
#define MAX_OPCODES     256
#define MAX_LABELS      4096

extern char *searchList[MAX_SEARCH_PATH];


FILE *searchPath(char *filename);


typedef union yylval_t {
    char *errorMsg;
    uint64_t number;
    char *name;
} yylval_t;

extern yylval_t yylval;




enum {
    TOK_EOL = 256,
    TOK_ERROR = 257,
    TOK_ARCH_REGISTER = 258,
    TOK_ARCH_NAME = 259,
    TOK_ARCH_NUMBER = 260,
    TOK_ARCH_MEMORY = 261,
    TOK_ARCH_OPCODE = 262,
    TOK_OPCODE_DB = 263,
    TOK_OPCODE_DEF = 264,
    TOK_BYTE_STREAM_DEF = 265,
    TOK_ORG = 266,
    TOK_NUMBER = 267,
    TOK_LABEL = 268,
    TOK_INSTRUCTION = 269,
};


extern char *sourceFile;
extern int yylineno;


void Fatal(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Error(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Warning(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Info(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Debug(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Msg(const char *msg, ...);


struct reg_t *AddRegister(char *r, int b);
void DumpRegisters(void);


void AddMemoryRange(uint64_t from, uint64_t to);
uint64_t GetStartingMemory(void);
uint64_t GetEndingMemory(void);
void DumpMemory(void);


void AddOpcode(char *def, char *bytes);
char *NormalizeInstruction(char *line);
void ParseInstruction(char *line);
void DumpOpcodes(void);


void BinaryInit(void);
void SetPosition(uint64_t p);
uint64_t GetLocation(void);
void SetAddressAt(uint64_t where, uint64_t address);
void EmitByte(uint8_t b);
void EmitInstruction(char *stream, uint64_t optionalArg);
void DumpBinary(uint64_t from, uint64_t to);
void OutputResults(void);

void NoteLabelLocation(void);
void DumpLabelLocations(void);

