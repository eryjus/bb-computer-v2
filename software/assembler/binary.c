

#include "asm.h"
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>

uint64_t location = 0;
int binaryInitialized = 0;

uint8_t *bin;


void BinaryInit(void)
{
    bin = malloc(GetEndingMemory() - GetStartingMemory() + 1);
    if (!bin) Fatal("Out of memory allocating binary storage", 0, 0, 0, 0);
    memset(bin, 0, GetEndingMemory() - GetStartingMemory() + 1);

    location = GetStartingMemory();
}

void SetPosition(uint64_t p)
{
    if (p < location) {
        Error("Cannot move backward in the output binary; location is already %lx\n",
                sourceFile,
                yylineno,
                0,
                0,
                location);
    } else if ((p < GetStartingMemory() && GetStartingMemory() != ~0) ||
            (p > GetEndingMemory() && GetEndingMemory() != 0)) {
        Error("`.org` location %lx is is outside memory range of %lx to %lx\n",
                sourceFile,
                yylineno,
                0,
                0,
                p,
                GetStartingMemory(),
                GetEndingMemory());
    } else {
        location = p;
    }
}


uint64_t GetLocation(void)
{
    return location;
}


void SetAddressAt(uint64_t where, uint64_t address)
{
    // -- TODO: get an address width and check the number of bytes to update
    // -- TODO: determine the endian-ness of the architecture and handle it here

    // -- for now, assume 16-bit addresses and little endian for all
    uint8_t lo = address & 0xff;
    uint8_t hi = (address >> 8) & 0xff;

    bin[where] = lo;
    bin[where + 1] = hi;
}


void EmitByte(uint8_t b)
{
    bin[location ++] = b;
}


void EmitInstruction(char *stream, uint64_t optionalArg)
{
    if (!binaryInitialized) {
        BinaryInit();
        binaryInitialized = 1;
    }

    for (int i = 0; i < strlen(stream); i ++) {
        if (stream[i] == '$' || stream[i + 1] == '(') {
            // -- handle the optional argument here
            i += 2;

            if (stream[i] == '8' && stream[i + 1] == ')') {
                EmitByte((uint8_t)(optionalArg & 0xff));
                i += 2;
                continue;
            }

            if (stream[i] == '1' && stream[i + 1] == '6' && stream[i + 2] == ')') {
                EmitByte((uint8_t)(optionalArg & 0xff));
                EmitByte((uint8_t)((optionalArg >> 8) & 0xff));
                i += 3;
                continue;
            }

            Error("Malformed instruction byte stream constant", 0, 0, 0, 0);
            return;
        }


        uint8_t b = 0;

        for (int j = 0; j < 2; j ++) {
            if (stream[i + j] >= 'a' && stream[i + j] <= 'z') {
                b = (b * 16) + stream[i + j] - 'a' + 10;
                continue;
            }

            if (stream[i + j] >= '0' && stream[i + j] <= '9') {
                b = (b * 16) + stream[i + j] - '0';
                continue;
            }

            Error("Incorrect 2-character hex in byte stream output", 0, 0, 0, 0);
        }

        EmitByte(b);

        i += 2;

        if (stream[i] != ' ' && stream[i] != 0) {
            Error("Byte Stream 2-character hex too long", 0, 0, 0, 0);
        }
    }
}


void DumpBinary(uint64_t from, uint64_t to)
{
    Msg("       x0 x1 x2 x3 x4 x5 x6 x7   x8 x9 xa xb xc xd xe xf");
    Msg("       -- -- -- -- -- -- -- --   -- -- -- -- -- -- -- --");
    for (uint64_t i = from; i <= to; i += 16) {
        Msg("%04xx: %02x %02x %02x %02x %02x %02x %02x %02x %c %02x %02x %02x %02x %02x %02x %02x %02x",
            i >> 4,
            bin[i +  0],
            bin[i +  1],
            bin[i +  2],
            bin[i +  3],
            bin[i +  4],
            bin[i +  5],
            bin[i +  6],
            bin[i +  7],
            ' ',
            bin[i +  8],
            bin[i +  9],
            bin[i + 10],
            bin[i + 11],
            bin[i + 12],
            bin[i + 13],
            bin[i + 14],
            bin[i + 15]);
    }
}



void OutputResults(void)
{
    FILE *fp = fopen("output.bin", "wb");
    fwrite(bin, GetEndingMemory() - GetStartingMemory() + 1, 1, fp);
    fclose(fp);
}
