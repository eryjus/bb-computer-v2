
#include "asm.h"
#include <stdint.h>
#include <sys/types.h>


#define MAX_MEM_RANGES 10

struct Mem_t {
    uint64_t from;
    uint64_t to;
};

uint64_t highestMem = 0;
uint64_t lowestMem = ~0;

struct Mem_t memRanges[MAX_MEM_RANGES] = { { 0 } };


uint64_t GetStartingMemory(void)
{
    return lowestMem;
}

uint64_t GetEndingMemory(void)
{
    return highestMem;
}


void AddMemoryRange(uint64_t from, uint64_t to)
{
    for (int i = 0; i < MAX_MEM_RANGES; i ++) {
        if (memRanges[i].from == 0 && memRanges[i].to == 0) {
            memRanges[i].from = from;
            memRanges[i].to = to;

            if (from < lowestMem) lowestMem = from;
            if (to > highestMem) highestMem = to;

            return;
        }

        if (from >= memRanges[i].from && from <= memRanges[i].to) {
            Error("Overlapping memory ranges defined for %lx to %lx\n", sourceFile, yylineno, 0, 0, from, to);
            return;
        }

        if (to <= memRanges[i].to && to >= memRanges[i].from) {
            Error("Overlapping memory ranges defined for %lx to %lx\n", sourceFile, yylineno, 0, 0, from, to);
            return;
        }
    }

    Error("Too many memory ranges defined\n", sourceFile, yylineno, 0, 0);
}



void DumpMemory(void)
{
    Msg("Overall Memory from %lx to %lx", lowestMem, highestMem);

    for (int i = 0; i < MAX_MEM_RANGES; i ++) {
        if (memRanges[i].from == 0 && memRanges[i].to == 0) {
            return;
        }

        Msg("Memory Range from %x to %x\n", memRanges[i].from, memRanges[i].to);
    }
}

