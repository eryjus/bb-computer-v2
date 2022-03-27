/* flex -i -o software/assembler/lexer.c software/assembler/lexer.ll */
/* clang -o software/assembler/asm software/assembler/lexer.c */

%{
    #include "asm.h"

    #define MAX_NESTED_INCLUDE  10
    YY_BUFFER_STATE includeStack[MAX_NESTED_INCLUDE];
    int lineNumberStack[MAX_NESTED_INCLUDE];
    char *sourceFileStack[MAX_NESTED_INCLUDE];
    char *currentSourceFile = "";

    int inclStackPtr = 0;

    int archDefined = 0;

    uint64_t ConvertNumber(int base, char *_s);

    #define HexNumber(yytext) ConvertNumber(16, yytext)
    #define OctNumber(yytext) ConvertNumber( 8, yytext)
    #define BinNumber(yytext) ConvertNumber( 2, yytext)
    #define DecNumber(yytext) ConvertNumber(10, yytext)
%}

%option yylineno
%option noyywrap
%option stack


%x              arch
%x              incl
%x              path

%x              opcode
%x              db

%x              architecture


WS              [ \t]

COMMENT         \;.*
NL              (\n|\r|\n\r|\r\n)

LETTER          [a-z_]
DIGIT           [0-9]
ALPHA           ({LETTER}|{DIGIT})
HEX             [0-9a-f]
OCT             [0-7]
BIN             [01]


%%

<arch>{WS}                          {}
<arch>{COMMENT}                     {}
<arch>{NL}                          { BEGIN(INITIAL); return TOK_EOL; }
<arch>[^ \t\n]+                     {
                                        if (inclStackPtr >= MAX_NESTED_INCLUDE) {
                                            fprintf(stderr,
                                                    "In file %s on line %d: Too many nested include files.\n",
                                                    (currentSourceFile?currentSourceFile:""),
                                                    yylineno);

                                            exit(EXIT_FAILURE);
                                        }

                                        includeStack[inclStackPtr] = YY_CURRENT_BUFFER;
                                        lineNumberStack[inclStackPtr] = yylineno;
                                        sourceFileStack[inclStackPtr] = currentSourceFile;
                                        inclStackPtr ++;

                                        yyin = searchPath(yytext);

                                        if (!yyin) {
                                            fprintf(stderr,
                                                    "In file %s on line %d: Unable to locate include file \"%s\"\n",
                                                    (currentSourceFile?currentSourceFile:""),
                                                    yylineno,
                                                    yytext);

                                            exit(EXIT_FAILURE);
                                        }

                                        currentSourceFile = strdup(yytext);
                                        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
                                        yylineno = 1;

                                        BEGIN(architecture);
                                    }



<incl>{WS}                          {}
<incl>{COMMENT}                     {}
<incl>{NL}                          { BEGIN(INITIAL); return TOK_EOL; }
<incl>[^ \t\n]+                     {
                                        if (inclStackPtr >= MAX_NESTED_INCLUDE) {
                                            fprintf(stderr,
                                                    "In file %s on line %d: Too many nested include files.\n",
                                                    (currentSourceFile?currentSourceFile:""),
                                                    yylineno);

                                            exit(EXIT_FAILURE);
                                        }

                                        includeStack[inclStackPtr] = YY_CURRENT_BUFFER;
                                        lineNumberStack[inclStackPtr] = yylineno;
                                        sourceFileStack[inclStackPtr] = currentSourceFile;
                                        inclStackPtr ++;

                                        yyin = searchPath(yytext);

                                        if (!yyin) {
                                            fprintf(stderr,
                                                    "In file %s on line %d: Unable to locate include file \"%s\"\n",
                                                    (currentSourceFile?currentSourceFile:""),
                                                    yylineno,
                                                    yytext);

                                            exit(EXIT_FAILURE);
                                        }

                                        currentSourceFile = strdup(yytext);
                                        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
                                        yylineno = 1;
                                    }


<path>{WS}                          {}
<path>{COMMENT}                     {}
<path>{NL}                          { BEGIN(INITIAL); return TOK_EOL; }
<path>[^ \t\n]+                     {
                                        char *buf = malloc(strlen(yytext) + 5);

                                        strcpy(buf, yytext);
                                        if (buf[strlen(buf) - 1] != '/') {
                                            int pos = strlen(buf);
                                            buf[pos] = '/';
                                            buf[pos + 1] = 0;
                                        }

                                        for (int i = 0; i < MAX_SEARCH_PATH; i ++) {
                                            if (!searchList[i]) {
                                                searchList[i] = buf;
                                                break;
                                            }
                                        }
                                    }




<architecture>{NL}                  { return TOK_EOL; }
<architecture>{WS}                  {}
<architecture>{COMMENT}             {}
<architecture>{LETTER}{ALPHA}*      {
                                        yylval.name = strdup(yytext);
                                        return TOK_ARCH_NAME;
                                    }
<architecture>0[Xx]{HEX}+           {
                                        yylval.number = HexNumber(yytext + 2);
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>0b{BIN}+              {
                                        yylval.number = BinNumber(yytext + 2);
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>0{OCT}*               {
                                        yylval.number = OctNumber(yytext);
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>{DIGIT}+              {
                                        yylval.number = DecNumber(yytext);
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>,                     { return ','; }
<architecture>\(                    { return '('; }
<architecture>\)                    { return ')'; }
<architecture>\+                    { return '+'; }
<architecture>\-                    { return '-'; }

<architecture>^\.register           { return TOK_ARCH_REGISTER; }
<architecture>^\.memory             { return TOK_ARCH_MEMORY; }
<architecture>^\.opcode             { yy_push_state(opcode); return TOK_ARCH_OPCODE; }

<architecture>\.db                  { yy_push_state(db); return TOK_OPCODE_DB; }

<architecture>.                     {
                                        yylval.errorMsg = "Unexpected character in .architecture definition file";
                                        return TOK_ERROR;
                                    }


<opcode>[^.\n]*                     {
                                        yylval.name = strdup(yytext);
                                        yy_pop_state();
                                        return TOK_OPCODE_DEF;
                                    }
<opcode>{NL}                        {
                                        yylval.errorMsg = "Unexpected newline in .opcode definition";
                                        yy_pop_state();
                                        return TOK_ERROR;
                                    }
<opcode>.                           {
                                        yylval.errorMsg = "Unexpected character in .opcode definition";
                                        yy_pop_state();
                                        return TOK_ERROR;
                                    }



<db>{COMMENT}                       {}
<db>[^;\n]*                         {
                                        yylval.name = strdup(yytext);
                                        yy_pop_state();
                                        return TOK_BYTE_STREAM_DEF;
                                    }
<db>{NL}                            {
                                        yylval.errorMsg = "Unexpected newline in .db byte code definition";
                                        yy_pop_state();
                                        return TOK_ERROR;
                                    }


{NL}                                { return TOK_EOL; }
{WS}                                {}
{COMMENT}.*                         {}

^\.include                          { BEGIN(incl); }
^\.path                             { BEGIN(path); }
^\.arch                             { BEGIN(arch); }

^{LETTER}{ALPHA}+\:                 {
                                        yylval.name = strdup(yytext);
                                        return TOK_LABEL;
                                    }
{LETTER}[^;\n]*                     {
                                        yylval.name = strdup(yytext);
                                        return TOK_INSTRUCTION;
                                    }

\.org                               { return TOK_ORG; }

{HEX}+                              {
                                        yylval.number = HexNumber(yytext);
                                        return TOK_NUMBER;
                                    }

<<EOF>>                             {
                                        -- inclStackPtr;

                                        if (inclStackPtr < 0) {
                                            yyterminate();
                                        } else {
                                            yy_delete_buffer(YY_CURRENT_BUFFER);
                                            yy_switch_to_buffer(includeStack[inclStackPtr]);
                                            yylineno = lineNumberStack[inclStackPtr];
                                            currentSourceFile = sourceFileStack[inclStackPtr];

                                            if (sourceFileStack[inclStackPtr]) {
                                                // TODO: should the pointer be freed?
                                                sourceFileStack[inclStackPtr] = 0;
                                            }
                                        }

                                        BEGIN(INITIAL);
                                    }

.                                   {
                                        Error("Unexpected character: %c\n",
                                                currentSourceFile,
                                                yylineno,
                                                0,
                                                0,
                                                yytext[0]);

                                        yylval.errorMsg = "Unexpected character";
                                        return TOK_ERROR;
                                    }



%%

void OpenFile (const char *file)
{
    extern FILE *yyin;

    yyin = fopen(file, "r");

    if (!yyin) {
        fprintf(stderr, "Unable to open source file\n");
        exit(EXIT_FAILURE);
    }

    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
    yylineno = 1;
}


uint64_t ConvertNumber(int base, char *_s)
{
    char *s = _s;
    uint64_t rv = 0;

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
            fprintf(stderr, "There was an error in the scanner parsing a number\n");
            exit(EXIT_FAILURE);
        }

        rv = rv * base + d;
        s ++;
    }

    return rv;
}
