//
// Created by Christian Curry on 7/10/15.
//

#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdlib.h>

char *fileBuffer;

uint8_t* memory;
uint16_t programCounter;
uint8_t accumulator;
uint8_t xIndex;
uint8_t yIndex;
uint8_t stackPointer;
uint16_t programStart;

//TODO: is 128 big enough? is this the best way to do this?
//the labels array will store the actual labels that the user creates
//the labelLocations array will store the actual memory locations to jump to
const char *labels[128];
uint8_t* labelLocations[128];
int labelIterator;

struct {
    uint8_t negative : 1;
    uint8_t overflow : 1;
    uint8_t ignored : 1;
    uint8_t breakFlag : 1;
    uint8_t decimal : 1;
    uint8_t interrupt : 1;
    uint8_t zero : 1;
    uint8_t carry : 1;

    //negative, overflow, ignored, break, decimal, interrupt, zero, carry

} flags;

enum addressingModes {
    IMMEDIATE,
    IMPLICIT,
    IMPLIED,
    ABSOLUTE,
    ZEROPAGE,
    ZEROPAGEY,
};

void executeInstruction(uint8_t instruction);
void executeProgram();
uint8_t getMachineCode(char* instruction, int addressingMode);
size_t loadFile(char* fileName);
void loadMemory(int secondByteUsed, int noArgument, uint8_t byteInstruction, uint8_t byteArgument1, uint8_t byteArgument2);
void determineInstruction(char argument[], char instruction[]);
void loadProgramIntoMemory(int fileLength);
void initializeCPU();

//eventually needs to be moved to a debugging source file
void printMemoryBlock(uint16_t start, uint16_t end);

#endif
