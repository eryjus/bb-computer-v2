
#include "asm.h"
#include <stdint.h>


struct LabelFixup_t {
    struct LabelFixup_t *next;
    uint64_t where;
};


struct Label_t {
    char *name;
    uint64_t location;
    struct LabelFixup_t *fixes;
};


struct Label_t labels[MAX_LABELS] = { { 0 } };


void FixupLocation(struct Label_t *lbl)
{
    struct LabelFixup_t *fix = lbl->fixes;

    while (fix) {
        SetAddressAt(fix->where, lbl->location);

        fix = fix->next;
    }

    lbl->fixes = 0;
}


void NoteLabelLocation(void)
{
    if (yylval.name[strlen(yylval.name) - 1] == ':') yylval.name[strlen(yylval.name) - 1] = 0;

    for (int i = 0; i < MAX_LABELS; i ++) {
        if (!labels[i].name) {
            labels[i].name = yylval.name;
            labels[i].location = GetLocation();
            labels[i].fixes = 0;

            return;
        }

        if (strcmp(labels[i].name, yylval.name) == 0) {
            if (labels[i].fixes) {
                FixupLocation(&labels[i]);
                return;
            }

            Error("Duplicate Label: %s", sourceFile, yylineno, 0, 0, yylval.name);
            return;
        }
    }
}



void DumpLabelLocations(void)
{
    for (int i = 0; i < MAX_LABELS; i ++) {
        if (!labels[i].name) {
            return;
        }

        Msg("Label `%s` is refers to address %x", labels[i].name, labels[i].location);

        if (labels[i].fixes) Error("Undefined labels %s", 0, 0, 0, 0, labels[i].name);
    }
}


