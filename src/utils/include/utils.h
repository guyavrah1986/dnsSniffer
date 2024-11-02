// ============================================================================
// ============================================================================
// utils.h: 
// This is the "module" (component) that holds all sort of general purpose
// utilities functions and definitions.
// 
// ============================================================================
// ============================================================================
#pragma once

#include <stdint.h>

// ==================
// Global definitions
// ==================
#define IN
#define OUT
#define INOUT

uint32_t hash(IN char* str);
void utilsPrintBytesAscii(IN const uint8_t* byteArr, IN const unsigned short numBytesToDisplay);
void utilsdisplayDnsName(IN const uint8_t* questionName);
void utilsGetCmdLineArgs(IN int argc, IN char** argv);