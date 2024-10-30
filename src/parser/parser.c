#include "include/parser.h"

#include <netinet/ip.h>
#include <netinet/udp.h>

int parseDnsPacket(const unsigned char *packet)
{
    // =============================================================================
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // TODO: In case DNS packets from NON Ethernet interfaces can arrive, then first
    // need to check that, for now, take into account only Ethernet frames
    // =============================================================================
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    const char funcName [] = "parseDnsPacket - ";
    
    
    // =============================================================================
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // TODO: In case DNS packets that are sent over IPv6 can arrive, then first
    // need to check that, for now, take into account only IPv4 packets
    // =============================================================================
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Get IPv4 header
    struct ip *ip_header = (struct ip *)(packet + 14); // Skip Ethernet header (14 bytes)
    int ip_header_len = ip_header->ip_hl * 4;

    unsigned char protocol = ip_header->ip_p;
    switch (protocol)
    {
        case IPPROTO_UDP:
            printf("%s DNS packet uses UDP as the transport layer\n", funcName);
            break;
        case IPPROTO_TCP:
            printf("%s DNS packet uses TCP as the transport layer\n", funcName);
            break;
        default: 
            printf("%s DNS packet arrived on non valid transport layer protocol (maybe the packet is coruppted)!\n", funcName);
            return -1;
    }
    // Get UDP header
    //struct udphdr *udp_header = (struct udphdr *)(packet + 14 + ip_header_len);
    int udp_header_len = sizeof(struct udphdr);

    // DNS packet starts after UDP header
    const unsigned char *dns_payload = packet + 14 + ip_header_len + udp_header_len;

    // DNS header is 12 bytes
    unsigned short dns_flags = ntohs(*(unsigned short *)(dns_payload + 2));

    if ((dns_flags & 0x8000) == 0)
    {
        printf("%s captured a DNS query packet\n", funcName);
        return 0;
    }
    else
    {
        printf("%s captured a DNS response packet\n", funcName);
        return 1;
    }
}

void packet_handler(unsigned char *user_data, const struct pcap_pkthdr *pkthdr, const unsigned char *packet)
{
    const char funcName [] = "packet_handler - ";
    if (NULL == user_data)
    {
        printf("%s the user_data argument is NULL\n", funcName);
    }

    printf("%s captured a packet with length of [%d]\n", funcName, pkthdr->len);
    // Additional processing on 'packet' can be done here
    int res = parseDnsPacket(packet);
    if (1 == res)
    {
        printf("%s captured DNS RESPONSE\n", funcName);
    }
    else
    {
        printf("%s captured DNS QUERY\n", funcName);
    }
}

int sampleFuncUsingLibpcap()
{
    const char funcName [] = "sampleFuncUsingLibpcap - ";
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
    pcap_loop(handle, numPacketsToCapture, packet_handler, NULL);

    // Close the handle
    pcap_close(handle);

    return 0;
}