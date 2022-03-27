
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int errorCount = 0;
int warningCount = 0;

int maxErrors = 1;
int warningsAreErrors = 0;



void Fatal(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "FATAL");

    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    }

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }

    exit(EXIT_FAILURE);
}



void Error(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "ERROR");

    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    } else fprintf(stderr, ":\n");

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }

    errorCount ++;

    if (errorCount >= maxErrors) {
        fprintf(stderr, "FATAL: Too many errors\n");
        exit(EXIT_FAILURE);
    }
}


void Warning(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "WARNING");

    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    } else fprintf(stderr, ":\n");

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }

    if (warningsAreErrors) {
        errorCount ++;

        if (errorCount >= maxErrors) {
            fprintf(stderr, "FATAL: Too many errors\n");
        }
    } else warningCount ++;
}


void Info(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "INFO");

    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    } else fprintf(stderr, ":\n");

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }
}



void Debug(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "DEBUG");

    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    } else fprintf(stderr, ":\n");

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }
}


void Msg(const char *msg, ...)
{
    va_list _list;
    va_start(_list, msg);
    vfprintf(stdout, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stdout, "\n");
    }
    va_end(_list);
}


