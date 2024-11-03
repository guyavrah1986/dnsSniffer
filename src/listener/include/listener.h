// ============================================================================
// ============================================================================
// listener.h: 
// This is the "module" (component) that is responsible to do all the work
// needed in order to listen to the relevant packets on the relevant interface
// of the system, be it wired Ethernet (802.3) or wireless WiFi (802.11x).
// 
// ============================================================================
// ============================================================================
#pragma once

#include <pcap.h>

#include "../../utils/include/utils.h"

// Global variables to share with other source files
// Shared flag to control the loop
volatile int keepRunning;
pcap_t* handle;

/*
This function is performing all the requiered settings and initialization
in order to start to listen on the main ("default") interface of the machine.
Return value:
the pcap_t handle - if all the preparations went well and the code can enter the
endless "capture loop".
NULL - otherwise.
*/
pcap_t* listenerPrepareToEnterRunLoop();

/*
This is the "main" endless capture loop. Once the code enters this function, it
starts to capture the desired packets (according to some filtering rules) from
the relevant interface.
*/
void* listenerRunLoop(IN void* arg);

/*
This function is responsible to perform all the relevant clean-up procedure
once the program terminates the "capture loop".
Return value:
0 - if all cleanup tasks went well
Non zero - otherwise.
*/
int listenerCleanupAfterRunLoop();

void packet_handler(IN unsigned char* user_data, IN const struct pcap_pkthdr* pkthdr, IN const unsigned char* packet);