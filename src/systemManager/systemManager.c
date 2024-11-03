#include <pthread.h>
#include <unistd.h>

#include "include/systemManager.h"
#include "../listener/include/listener.h"
#include "../dataBaseManager/include/dataBaseManager.h"
#include "../utils/include/hashTable.h"

static void systemMgrDisplayUserDomainInfo(IN const char* domainNameToQuery)
{
    const char funcName [] = "systemMgrDisplayUserDomainInfo -";
    struct node* listToDisplay = dataBaseMgrGetItem(domainNameToQuery);
    if (NULL == listToDisplay)
    {
        printf("%s no addresses were captured for domain name:%s\n", funcName, domainNameToQuery);
        return;
    }

    printf("%s domain name:%s has the following addresses\n", funcName, domainNameToQuery);
    display_list(listToDisplay);
}

/*
This function is the "main UI" function. Its objective is to be the "GUI"
towards the user that runs this program from a terminal.
It starts after the system did all of its initializations procedure such
as intialize the DB, starting the capture on the main interface, etc...
It asks the user to enter a domain name that he wish to query and displays
to the screen the reply from the program (i.e. - relevant IP addresses, if any
are present in the DB).
This function runs in its own thread, AKA the "UI thread".
Once the user enters the termination key, the function cancles the pcap loop
thread and terminates 
*/
static void* systemMgrDisplayClientMenu(IN void* arg)
{
    const char funcName [] = "systemMgrDisplayClientMenu -";
    printf("%s system is listening to DNS responses\n", funcName);
    char input[256];
    pthread_t* pcapLoopThread = (pthread_t*)arg;

    while (1)
    {
        printf("%s enter a domain name to query or type 'stop' to end: ", funcName);

        // Read user input from the terminal
        if (NULL == fgets(input, sizeof(input), stdin))
        {
            // If fgets fails, we should break to avoid an infinite loop
            printf("%s error reading input. Exiting...\n", funcName);
            pthread_cancel(*pcapLoopThread); 
            return NULL;
        }

        // Remove the newline character at the end, if any
        input[strcspn(input, "\n")] = '\0';

        // Check if the user entered "stop"
        if (0 == strcmp(input, "stop"))
        {
            printf("%s signalling to stop the capture loop.\n", funcName);
            
            // Signal to stop capturing
            pthread_cancel(*pcapLoopThread); 
            break;
        }

        // Otherwise, print the domain name entered
        printf("%s you entered: %s\n", funcName, input);
        systemMgrDisplayUserDomainInfo(input);
    }

    printf("%s done with client UI loop\n", funcName);
    return NULL;
}

static int systemMgrInit()
{
    const char funcName [] = "systemMgrInit - ";
    int ret = dataBaseMgrInit();
    if (0 != ret)
    {
        printf("%s was unable to initialize the system correctly, aborting\n", funcName);
        return 1;
    }
    
    return 0;
}

static void systemMgrTerminate()
{
    dataBaseMgrClean();
}

int systemMgrRunProgram()
{
    const char funcName [] = "systemMgrRunProgram -";
    pthread_t listenLoopThread, uiThread;

    // First, perform all the requiered init of the system
    int ret = systemMgrInit();
    if (0 != ret)
    {
        return -1;
    }

    pcap_t* handleCopy = listenerPrepareToEnterRunLoop();
    if (NULL == handleCopy)
    {
        printf("%s the listenerPrepareToEnterRunLoop function was not successfull, aborting\n", funcName);
        return -1;
    }

    printf("%s about to create the listen loop thread\n", funcName);
    if (0 != pthread_create(&listenLoopThread, NULL, listenerRunLoop, &handleCopy))
    {
        printf("%s failed to create listen loop thread, aborting\n", funcName);
        return -1;
    }

    printf("%s about to create the UI loop thread\n", funcName);
    if (0 != pthread_create(&uiThread, NULL, systemMgrDisplayClientMenu, &listenLoopThread))
    {
        printf("%s failed to create UI loop thread, aborting\n", funcName);
        return -1;
    }

    printf("%s back on the main thread, after two threads started\n", funcName);
    if (0 != pthread_join(uiThread, NULL))
    {
        printf("%s failed to join the UI thread after it is done\n", funcName);
    }

    if (0 != pthread_join(listenLoopThread, NULL))
    {
        printf("%s failed to join the listen loop thread after it is done\n", funcName);
    }

    printf("%s back on the main thread, after joining both threads\n", funcName);

    // ==================================
    // ==================================
    // Clean ups upon program termination
    // ==================================
    // ==================================
    ret = listenerCleanupAfterRunLoop();
    if (0 != ret)
    {
        printf("%s the listenerCleanupAfterRunLoop function was not successfull, aborting\n", funcName);
        return -1;
    }

    printf("%s before calling systemMgrTerminate\n", funcName);
    systemMgrTerminate();
    return ret;
}
