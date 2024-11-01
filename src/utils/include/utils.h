#pragma once

#include <stdint.h>

// ==================
// Global definitions
// ==================
#define IN
#define OUT
#define INOUT

void printBytesAscii(const uint8_t* byteArr, const unsigned short numBytesToDisplay);
void displayDNSName(const uint8_t* questionName);
void getCmdLineArgs(int argc, char** argv);