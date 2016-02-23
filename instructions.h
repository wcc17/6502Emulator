//
// Created by Christian Curry on 7/15/15.
//

#ifndef EMULATOR_INSTRUCTIONS_H
#define EMULATOR_INSTRUCTIONS_H

#include "6502.h"
#include "util.h"

void loadAccumulatorImmediate();
void storeAccumulatorAbsolute();
void storeAccumulatorZeroPage();
void storeXRegisterAbsolute();
void storeXRegisterZeroPage();
void transferAccumulatorToXIndex();
void transferAccumulatorToYIndex();
void transferXToAccumulator();
void transferYToAccumulator();
void incrementXIndex();
void decrementXIndex();
void addMemoryToAccumulatorWithCarryImmediate();
void addMemoryToAccumulatorWithCarryZeroPage();
void subtractMemoryFromAccumulatorWithCarryZeroPage();
void subtractMemoryFromAccumulatorWithCarryImmediate();
void breakInstruction();

#endif
