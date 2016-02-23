//
// Created by Christian Curry on 7/10/15.
//

#include "6502.h"
#include "util.h"
#include "instructions.h"

#include <stdio.h>
#include <stdbool.h>

void executeInstruction(uint8_t instruction) {
    switch(instruction) {
        case 0x00:
            breakInstruction();
            break;
        case 0x65:
            addMemoryToAccumulatorWithCarryZeroPage();
            break;
        case 0x69:
            addMemoryToAccumulatorWithCarryImmediate();
            break;
        case 0x8a:
            transferXToAccumulator();
            break;
        case 0x85:
            storeAccumulatorZeroPage();
            break;
        case 0x8d:
            storeAccumulatorAbsolute();
            break;
        case 0x98:
            transferYToAccumulator();
            break;
        case 0xa9:
            loadAccumulatorImmediate();
            break;
        case 0xaa:
            transferAccumulatorToXIndex();
            break;
        case 0xa8:
            transferAccumulatorToYIndex();
            break;
        case 0xca:
            decrementXIndex();
            break;
        case 0xe5:
            subtractMemoryFromAccumulatorWithCarryZeroPage();
            break;
        case 0xe8:
            incrementXIndex();
            break;
        case 0xe9:
            subtractMemoryFromAccumulatorWithCarryImmediate();
            break;

    }
}

void executeProgram() {
    programCounter = programStart;
    int endOfProgram = 0;

    while(endOfProgram == 0) {
        uint8_t currentInstruction = memory[programCounter++];

        //TODO: get rid of
        int pc_bitch = programCounter;

        if(currentInstruction == 0x00) {
            endOfProgram = 1;
        }
        else {
            executeInstruction(currentInstruction);
        }
    }
}

uint8_t getMachineCode(char* instruction, int addressingMode) {
    if( strcmp(instruction, "LDA") == 0) {
        switch(addressingMode) {
            case IMMEDIATE:
                return 0xa9;
        }
    }
    else if( strcmp(instruction, "STA") == 0) {
        switch (addressingMode) {
            case ABSOLUTE:
                return 0x8d;
            case ZEROPAGE:
                return 0x85;
        }
    }
    else if( strcmp(instruction, "STX") == 0) {
        switch (addressingMode) {
            case ABSOLUTE:
                return 0x8e;
            case ZEROPAGE:
                return 0x86;
            case ZEROPAGEY:
                return 0x96;
        }
    }
    else if( strcmp(instruction, "TAX") == 0) {
        return 0xaa;
    }
    else if( strcmp(instruction, "TAY") == 0) {
        return 0xa8;
    }
    else if( strcmp(instruction, "TXA") == 0) {
        return 0x8a;
    }
    else if( strcmp(instruction, "TYA") == 0) {
        return 0x98;
    }
    else if( strcmp(instruction, "INX") == 0) {
        return 0xe8;
    }
    else if( strcmp(instruction, "DEX") == 0) {
        return 0xca;
    }
    else if( strcmp(instruction, "ADC") == 0) {
        switch(addressingMode) {
            case ZEROPAGE:
                return 0x65;
            case IMMEDIATE:
                return 0x69;
        }
    }
    else if( strcmp(instruction, "SBC") == 0) {
        switch(addressingMode) {
            case ZEROPAGE:
                return 0xe5;
            case IMMEDIATE:
                return 0xe9;
        }
    }
    else if( strcmp(instruction, "BRK") == 0) {
        return 0x00;
    }

    return 0x00;
}

void printMemoryBlock(uint16_t start, uint16_t end) {
    int x = 0;

    //printf("%04x:  ", start);
    for(int i = start; i <= end; i++) {
        printf("%02x  ", memory[i]);

        x++;
        if(x % 16 == 0) {
            printf("\n");
            //printf("%04x:  ", i);
        }

    }
}

//loads hardcoded file and returns size of file
size_t loadFile(char* fileName) {

    FILE *fp;
    size_t fileLength;

    fp = fopen(fileName, "rt");

    if(fp) {
        fseek(fp, 0, SEEK_END); //end of file
        fileLength = ftell(fp); //ftell gets current position of stream
        fseek(fp, 0, SEEK_SET); //beginning of file
        fileBuffer = malloc (fileLength);

        if(fileBuffer) {
            fread(&fileBuffer[0x0000], 1, fileLength, fp);
        }

        fclose(fp);
    }

    return fileLength;
}

void loadMemory(int secondByteUsed, int noArgument, uint8_t byteInstruction, uint8_t byteArgument1, uint8_t byteArgument2) {

    if(noArgument == 1) {
        memory[programCounter++] = byteInstruction;
    }
    else if (secondByteUsed == 0) {
        memory[programCounter++] = byteInstruction;
        memory[programCounter++] = byteArgument1;
    }
    else {
        memory[programCounter++] = byteInstruction;
        memory[programCounter++] = byteArgument1;
        memory[programCounter++] = byteArgument2;
    }
}

void determineInstruction(char argument[], char instruction[]) {
    uint8_t byteInstruction;
    uint8_t byteArgument1;
    uint8_t byteArgument2 = NULL;

    int i = 0;
    char c = instruction[i];
    bool isLabel = false;

    while(c != '\0') {

        if(c == ':') {
            isLabel = true;
            instruction[i] = '\0';
            c = instruction[i];
        } else {
            c = instruction[++i];
        }

    }

    if(!isLabel) {
        int secondByteUsed = 0;
        int noArgument = 0;

        if (strlen(argument) < 1) {
            byteInstruction = getMachineCode(instruction, IMPLIED);

            noArgument = 1;
        }
            //FORMAT: LDA #$XX
        else if (argument[0] == '#') {
            char byte[3];
            char high = argument[2];
            char low = argument[3];

            byte[0] = high;
            byte[1] = low;
            byte[2] = '\0';

            byteArgument1 = stringToByte(byte);
            byteInstruction = getMachineCode(instruction, IMMEDIATE);

//        printf("Instruction used: %02x\n", byteInstruction);
//        printf("Value to load in accumulator: %02x\n\n", byteArgument1);
        }
            //FORMAT: STA $XXXX OR STA $XX
        else if (argument[0] == '$') {
            //if argument is 4 digits (0x0000)
            if(strlen(argument) > 3) {
                //need to get two memory locations here instead of just one
                char byteHigh[3];
                char byteLow[3];

                //get first part of the memory address
                char high = argument[1];
                char low = argument[2];
                byteHigh[0] = high;
                byteHigh[1] = low;

                high = argument[3];
                low = argument[4];
                byteLow[0] = high;
                byteLow[1] = low;

                byteLow[2] = '\0';
                byteHigh[2] = '\0';

                //little endian, low comes first
                byteArgument1 = stringToByte(byteLow);
                byteArgument2 = stringToByte(byteHigh);
                byteInstruction = getMachineCode(instruction, ABSOLUTE);

                secondByteUsed = 1;
            }
                //if argument is 2 digits (0x00) for zero page, etc.
            else {
                char byte[3];
                char high = argument[1];
                char low = argument[2];
                byte[0] = high;
                byte[1] = low;
                byte[2] = '\0';

                byteArgument1 = stringToByte(byte);
                byteInstruction = getMachineCode(instruction, ZEROPAGE);

                secondByteUsed = 0;
            }

        }
        else if (argument[0] == '(') {
            printf("new addressing shit pt 2\n\n");
        }
//        else if (argument[0] == 'A') {
//            printf("new addressing shit pt 3\n\n");
//        }
        else {
            //this is a label
            for(int i = 0; i < labelIterator; i++) {
                if(labels[i] == argument) {
                    uint8_t currentPC = programCounter;

                    //we have the location of where the label should start
                    //we need an offset from the next instruciton to that location
                    //so we increase programCounter by 2 (1 for this instruction, 1 for this argument)
                    //then subtract the location of the label from the program counter
                    //we now have the offset
                    //subtract offset from 256 now
                    currentPC += 2;
                    uint8_t location = labelLocations[i];
                    uint8_t offset = currentPC - location;

                    byteArgument1 = 256 - offset;
                    printf('test');
                }
            }
        }

        loadMemory(secondByteUsed, noArgument, byteInstruction, byteArgument1, byteArgument2);
    } else {
        //get the location of the next instruction to be executed
        uint8_t memoryLocation = programCounter + 1;
        labels[labelIterator] = instruction;
        labelLocations[labelIterator] = memoryLocation;
        labelIterator += 1;
    }
}

void loadProgramIntoMemory(int fileLength) {
    int i = 0;

    while(i < fileLength) {

        //TODO: FIND THE LONGEST POSSIBLE INSTRUCTION + ADDRESS COMBINATION TO SET THIS
        char instruction[10];
        char argument[10];

        char c = fileBuffer[i];

        while ((c != ' ') && (c != '\n') && (i < fileLength)) {
            append(instruction, c);

            i++;
            c = fileBuffer[i];
        }

        c = fileBuffer[++i];

        while ( (c != '\n') && (c != ' ') && (i < fileLength) ) {
            append(argument, c);

            i++;
            c = fileBuffer[i];
        }

        //if we encounter a space after the argument
        while(c == ' ') {
            c = fileBuffer[++i];
        }
        while(c == '\n') {
            c = fileBuffer[++i];
        }

        if(c == ';') {
            while(c != '\n') {
                c = fileBuffer[++i];

                if(c == '\n') {
                    i++;
                    break;
                }
            }
        }

        determineInstruction(argument, instruction);

        memset(instruction, 0, 10);
        memset(argument, 0, 10);
    }
}

void initializeCPU() {
    memory = malloc(65535);
    programCounter = 0x0600;
    programStart = 0x0600;
    flags.ignored = 1;
    flags.breakFlag = 1;

    labelIterator = 0;
}

    //this is a perfect way to load the nes rom.
    //doesn't work for hex already or code written in the 6502 language
//    if(fileBuffer) {
//        unsigned char opcode;
//        opcode = fileBuffer[0];
//
//        printf("%02x\n", fileBuffer[0]);
//        printf("%02x\n", fileBuffer[1]);
//        printf("%02x\n", fileBuffer[2]);
//        printf("%02x\n", fileBuffer[3]);
//        printf("%02x\n", fileBuffer[4]);
//        printf("%02x\n", fileBuffer[5]);
//        printf("%02x\n", fileBuffer[6]);
//    }
