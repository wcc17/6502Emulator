//
// Created by Christian Curry on 7/15/15.
//

#include "util.h"

//128 = 10000000
//128 & 0x84 (0x84 == 132 == 10000100) = 10000000. so c & mask don't equal zero. so method will return 1.
int checkNthBitSet (unsigned char c, int n) {
    static unsigned char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
    return ((c & mask[n]) != 0);
}

uint8_t charToHexDigit(char c)
{
    if (c >= 'a') {
        uint8_t a = (c - 'a' + 10);
        return a;
    }
    else {
        uint8_t b = (c - '0');
        return b;
    }
}

uint8_t stringToByte(char c[2])
{
    uint8_t a = (charToHexDigit(c[0]) * 16 + charToHexDigit(c[1]));
    return a;
}

void append(char* s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}
