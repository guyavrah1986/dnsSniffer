#include <pcap.h>

#include "include/listener.h"

// GuyA: This rule can be set instead in some configuration file and/or passed
// to the program via command line argument. For now, set it hard-coded here.
// According to the requierments, the following need to be met:
// 1. The DNS transport layer of should be only UDP  --> udp port 53
// 2. We are only interested in DNS responses        --> udp[10] & 0x80 != 0
// 3. We are only interested in successful responses --> udp[11] & 0x0F == 0
// NOTE: Filtering for responses with specific types of records, such as AAAA
// for example, is not possible using libcpap filtering.
static const char filter_exp[] = "(udp port 53) && (udp[10] & 0x80 != 0) && (udp[11] & 0x0F == 0)";

// Global variables of this module:
static pcap_t *handle;
static pcap_if_t *all_devices;
static struct bpf_program fp;

int listenerPrepareToEnterRunLoop()
{
    const char funcName [] = "listenerPrepareToEnterRunLoop - ";
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *first_device;

    printf("%s about to look for devices (interfaces) to listen on\n", funcName);

    // Find the default network device to capture on
    if (-1 == pcap_findalldevs(&all_devices, errbuf))
    {
        printf("%s couldn't find default device:%s\n", funcName, errbuf);
        return 1;
    }

    // Check if we have at least one device
    if (NULL == all_devices)
    {
        printf("%s no devices found\n", funcName);
        return 1;
    }

    // Get the first device in the list, this is to be assumed the main
    // ("defualt") interface
    first_device = all_devices;

    // Print the name of the first device
    printf("%s the available device is:%s\n", funcName, first_device->name);

    // Open the device for capturing (promiscuous mode)
    handle = pcap_open_live(first_device->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL)
    {
        printf("%s couldn't open device %s: %s\n", funcName, first_device->name, errbuf);
        return 2;
    }

    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
    {
        printf("%s couldn't parse filter %s: %s\n", funcName, filter_exp, pcap_geterr(handle));
        return 3;
    }

    if (-1 == pcap_setfilter(handle, &fp))
    {
        printf("%s couldn't install filter %s: %s\n", funcName, filter_exp, pcap_geterr(handle));
        return 3;
    }

    printf("%s all preparations were successful\n", funcName);
    return 0;
}

void listenerRunLoop()
{
    const char funcName [] = "listenerRunLoop - ";
    printf("%s about to start and sniff DNS packets\n", funcName);
    pcap_loop(handle, 2, packet_handler, NULL);
}

int listenerCleanupAfterRunLoop()
{
    const char funcName [] = "listenerCleanupAfterRunLoop - ";
    pcap_freealldevs(all_devices);
    pcap_freecode(&fp);
    pcap_close(handle);
    printf("%s released all relevant resources successfully\n", funcName);
    return 0;
}

void packet_handler(unsigned char* user_data, const struct pcap_pkthdr* pkthdr, const unsigned char* packet)
{
    const char funcName [] = "packet_handler - ";
    if (NULL == user_data || NULL == pkthdr || NULL == packet)
    {
        printf("%s one or more of the provided argument(s)  is NULL\n", funcName);
        return;
    }
}
