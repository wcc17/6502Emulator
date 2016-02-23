//
// Created by Christian Curry on 7/15/15.
//

#ifndef EMULATOR_UTIL_H
#define EMULATOR_UTIL_H

#include <stdlib.h>
#include <string.h>

int checkNthBitSet (unsigned char c, int n);
uint8_t charToHexDigit(char c);
uint8_t stringToByte(char c[2]);
void append(char* s, char c);

#endif
