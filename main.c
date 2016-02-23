//
// Created by Christian Curry on 7/15/15.
//

#include "6502.h"
#include <stdio.h>

int main() {
    initializeCPU();

    size_t fileLength;
    fileLength = loadFile("eighthprogram.asm");

    if (fileBuffer) {

        loadProgramIntoMemory(fileLength);

        executeProgram();

        printf("memory where program is actually located:\n");
        printMemoryBlock(0x0600, 0x06ff);
        printf("\n");

        printf("memory where program is chasnging things:\n");
        printMemoryBlock(0x0000, 0x0300);
        printf("\n");

        printf("accumulator: %02x\n", accumulator);
        printf("x index: %02x\n", xIndex);
        printf("y index: %02x\n", yIndex);
        printf("program counter: %04x\n", programCounter);
        printf("flags: N V - B D I Z C\n");
        printf("       %d %d %d %d %d %d %d %d", flags.negative, flags.overflow, flags.ignored, flags.breakFlag, flags.decimal, flags.interrupt, flags.zero, flags.carry);
    }

    return 0;
}
