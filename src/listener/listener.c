#include <pcap.h>

#include "include/listener.h"

void packet_handler(unsigned char* user_data, const struct pcap_pkthdr* pkthdr, const unsigned char* packet)
{
    const char funcName [] = "packet_handler - ";
    if (NULL == user_data || NULL == pkthdr || NULL == packet)
    {
        printf("%s one or more of the provided argument(s)  is NULL\n", funcName);
        return;
    }
}

int listenerRunLoop()
{
    const char funcName [] = "listenerRunLoop - ";
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    pcap_if_t *all_devices;
    pcap_if_t *first_device;

    printf("%s about to look for devices (interfaces) to listen on\n", funcName);
    // Find the default network device to capture on
    //dev = pcap_lookupdev(errbuf);
    if (-1 == pcap_findalldevs(&all_devices, errbuf))
    {
        printf("%s couldn't find default device: %s\n", funcName, errbuf);
        return 1;
    }

    // Check if we have at least one device
    if (NULL == all_devices)
    {
        printf("%s no devices found\n", funcName);
        return 1;
    }

    // Get the first device in the list
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

    // TODO: Add similar filter rule for TCP
    // Apply the filter to capture only DNS packets which
    // satisify the following conditions:
    // 1. DNS response (the relevant bit in the QR filed is set to 1)
    // 2. Return code is successfull (RCode field is set to 0)
    // 3. Either UDP or TCP.
    // NOTE: Filtering for responses with specific types of records, such as AAAA
    // for example, is not possible using libcpap filtering.
    struct bpf_program fp;
    const char filter_exp[] = "(udp port 53) && (udp[10] & 0x80 != 0) && (udp[11] & 0x0F == 0)";
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
    {
        printf("%s couldn't parse filter %s: %s\n", funcName, filter_exp, pcap_geterr(handle));
        return 1;
    }

    if (pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return 1;
    }

    // Capture packets in an infinite loop (can set to specific number if desired)
    int numPacketsToCapture = 2;
    printf("%s about to sniff %d packets and then terminate\n", funcName, numPacketsToCapture);
    pcap_loop(handle, numPacketsToCapture, packet_handler, NULL);

    // Cleanup
    pcap_freealldevs(all_devices);
    pcap_freecode(&fp);
    pcap_close(handle);
    return 0;
}