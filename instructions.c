//
// Created by Christian Curry on 7/15/15.
//

#include "instructions.h"
#include "6502.h"
#include <stdio.h>

void loadAccumulatorImmediate() {
    uint8_t argument = memory[programCounter++];

    accumulator = argument;

    if (accumulator == 0) {
        flags.zero = 1;
    }
    else {
        flags.zero = 0;
    }

    if (checkNthBitSet(accumulator, 0) == 1) {
        flags.negative = 1;
    }
    else if(flags.negative == 1) {
        flags.negative = 0;
        flags.overflow = 1;
    }
    else {
        flags.overflow = 0;
    }

    printf("LDA. accumulator = %02x\n", accumulator);
}

void storeAccumulatorAbsolute() {
    uint8_t arg1 = memory[programCounter++];
    uint8_t arg2 = memory[programCounter++];

    //if you need to know how this works again:
    //http://stackoverflow.com/questions/15249791/combining-two-uint8-t-as-uint16-t
    uint16_t argument = ((uint16_t) arg2 << 8) | arg1;

    memory[argument] = accumulator;
}

void storeAccumulatorZeroPage() {
    uint8_t arg = memory[programCounter++];

    memory[arg] = accumulator;
}

void storeXRegisterAbsolute() {
    
}

void storeXRegisterZeroPage() {

}

void transferAccumulatorToXIndex() {
    xIndex = accumulator;

    if (checkNthBitSet(xIndex, 0) == 1) {
        flags.negative = 1;
    }
}

void transferAccumulatorToYIndex() {
    yIndex = accumulator;

    if (checkNthBitSet(yIndex, 0) == 1) {
        flags.negative = 1;
    }
}

void transferXToAccumulator() {
    accumulator = xIndex;

    if (checkNthBitSet(accumulator, 0) == 1) {
        flags.negative = 1;
    }
}

void transferYToAccumulator() {
    accumulator = yIndex;

    if (checkNthBitSet(accumulator, 0) == 1) {
        flags.negative = 1;
    }
}

void incrementXIndex() {
    xIndex++;
    if(xIndex > 0xff) {
        xIndex = 0x00;
    }

    if(xIndex == 0x00) {
        flags.zero = 1;
    } else {
        flags.zero = 0;
    }

    if(checkNthBitSet(xIndex, 0) == 1) {
        flags.negative = 1;
    } else {
        flags.negative = 0;
    }
}

void decrementXIndex() {
    xIndex--;
}

//ADC
void addMemoryToAccumulatorWithCarryImmediate() {
    uint8_t arg = memory[programCounter++];

    //if we have a carry, it needs to be added to the result
    int carry = 0;
    if(flags.carry == 1) {
        carry = 1;
    }

    //the uint8 will only go to 0xff. so if we add two numbers and the sum is larger than 0xff
    //itll drop the carry. the uint16 wont, so we use that to check if teh carry flag should be set.
    uint16_t checkCarry = accumulator + arg + carry;
    accumulator = accumulator + arg + carry;

    if(checkCarry > 0xff) {
        flags.carry = 1;
    }
    else {
        flags.carry = 0;
    }

    if(checkNthBitSet(accumulator, 0) == 1) {
        flags.negative = 1;
    }
    else if(flags.negative == 1) {
        flags.negative = 0;
        flags.overflow = 1;
    }
    else {
        flags.overflow = 0;
    }

    if(accumulator == 0) {
        flags.zero = 1;
    }
    else {
        flags.zero = 0;
    }

    printf("ADC. accumulator = %02x\n", accumulator);
}

void addMemoryToAccumulatorWithCarryZeroPage() {
    uint8_t memoryLocation = memory[programCounter++];
    uint8_t arg = memory[memoryLocation];

    //if we have a carry, it needs to be added to the result
    int carry = 0;
    if(flags.carry == 1) {
        carry = 1;
    }

    uint16_t checkCarry = accumulator + arg + carry;
    accumulator = accumulator + arg + carry;

    if(checkCarry > 0xff) {
        flags.carry = 1;
    }
    else {
        flags.carry = 0;
    }

    if(checkNthBitSet(accumulator, 0) == 1) {
        flags.negative = 1;
    }
    else if(flags.negative == 1) {
        flags.negative = 0;
        flags.overflow = 1;
    }
    else {
        flags.overflow = 0;
    }

    if(accumulator == 0) {
        flags.zero = 1;
    }
    else {
        flags.zero = 0;
    }

    printf("ADC. accumulator = %02x\n", accumulator);
}

void subtractMemoryFromAccumulatorWithCarryImmediate() {

}

void subtractMemoryFromAccumulatorWithCarryZeroPage() {

}

void breakInstruction() {

}
