#include <pcap.h>

#include "include/listener.h"
#include "../parser/include/parser.h"

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
static pcap_if_t *all_devices;
static struct bpf_program fp;

static void packet_handler(IN unsigned char* userData, IN const struct pcap_pkthdr* pkthdr, IN const unsigned char* packet)
{
    const char funcName [] = "packet_handler - ";
    if (NULL == userData)
    {
        printf("%s userData is NULL (but it is not indeed an issue)\n", funcName);
    }

    if (NULL == pkthdr)
    {
        printf("%s pkthdr is NULL\n", funcName);
        return;
    }

    if (NULL == packet)
    {
        printf("%s packet is NULL, aborting processing of this packet\n", funcName);
        return;
    }

    // Calculate the offset from the begining of the captured frame all the 
    // way to the DNS payload
    // GuyA: TODO - check also IPv6 and/or IPv4 with options section!!!
    int ret = parseDnsResponse(packet);
    printf("%s parseDnsResponse returned:%d\n", funcName, ret);
}

pcap_t* listenerPrepareToEnterRunLoop()
{
    const char funcName [] = "listenerPrepareToEnterRunLoop - ";
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *first_device;

    printf("%s about to look for devices (interfaces) to listen on\n", funcName);

    // Find the default network device to capture on
    if (-1 == pcap_findalldevs(&all_devices, errbuf))
    {
        printf("%s couldn't find default device:%s\n", funcName, errbuf);
        return NULL;
    }

    // Check if we have at least one device
    if (NULL == all_devices)
    {
        printf("%s no devices found\n", funcName);
        return NULL;
    }

    // Get the first device in the list, this is to be assumed the main
    // ("defualt") interface
    first_device = all_devices;

    // Print the name of the first device
    printf("%s the available device is:%s\n", funcName, first_device->name);

    // Open the device for capturing (promiscuous mode)
    handle = pcap_open_live(first_device->name, BUFSIZ, 1, 1000, errbuf);
    if (NULL == handle)
    {
        printf("%s couldn't open device %s: %s\n", funcName, first_device->name, errbuf);
        return NULL;
    }

    if (-1 == pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN))
    {
        printf("%s couldn't parse filter %s: %s\n", funcName, filter_exp, pcap_geterr(handle));
        return NULL;
    }

    if (-1 == pcap_setfilter(handle, &fp))
    {
        printf("%s couldn't install filter %s: %s\n", funcName, filter_exp, pcap_geterr(handle));
        return NULL;
    }

    printf("%s all preparations were successful\n", funcName);
    return handle;
}

void* listenerRunLoop(IN void* arg)
{
    const char funcName [] = "listenerRunLoop - ";
    if (NULL == arg)
    {
        printf("%s got arg as a NULL pointer\n", funcName);
        return NULL;
    }

    printf("%s about to start and sniff DNS packets\n", funcName);
    pcap_loop(handle, -1, packet_handler, NULL);
    printf("%s after pcap_loop\n", funcName);
    return NULL;
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

