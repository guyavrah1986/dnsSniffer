#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>

#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

// Utilities:

int parseDnsPacket(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    // Get IP header
    struct ip *ip_header = (struct ip *)(packet + 14); // Skip Ethernet header (14 bytes)
    int ip_header_len = ip_header->ip_hl * 4;

    // Get UDP header
    struct udphdr *udp_header = (struct udphdr *)(packet + 14 + ip_header_len);
    int udp_header_len = sizeof(struct udphdr);

    // DNS packet starts after UDP header
    const u_char *dns_payload = packet + 14 + ip_header_len + udp_header_len;

    // DNS header is 12 bytes
    unsigned short dns_flags = ntohs(*(unsigned short *)(dns_payload + 2));

    if ((dns_flags & 0x8000) == 0)
    {
        printf("Captured a DNS query packet\n");
        return 0;
    }
    else
    {
        printf("Captured a DNS response packet\n");
        return 1;
    }
}

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    const char funcName [] = "packet_handler - ";
    printf("%s captured a packet with length of [%d]\n", funcName, pkthdr->len);
    // Additional processing on 'packet' can be done here
    int res = parseDnsPacket(user_data, pkthdr, packet);
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
    char *dev, errbuf[PCAP_ERRBUF_SIZE];
    const char funcName [] = "sampleFuncUsingLibpcap - ";
    pcap_t *handle;

    printf("%s about to look for devices (interfaces) to listen on\n", funcName);
    // Find the default network device to capture on
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL)
    {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        return 1;
    }

    printf("%s using device %s for packet capture\n", funcName, dev);

    // Open the device for capturing (promiscuous mode)
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL)
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 2;
    }

    // Apply the filter to capture only DNS packets
    struct bpf_program fp;
    char filter_exp[] = "udp port 53";
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return 1;
    }

    if (pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return 1;
    }

    // Capture packets in an infinite loop (can set to specific number if desired)
    int numPacketsToCapture = 2;
    pcap_loop(handle, numPacketsToCapture, packet_handler, NULL); // Capture 10 packets

    // Close the handle
    pcap_close(handle);

    return 0;
}

int main(int argc, char** argv)
{
    const char funcName [] = "main - ";
    printf("%s START\n", funcName);

    int ret = sampleFuncUsingLibpcap();
    printf("%s the sample function returned:%d\n", funcName, ret);
    
    printf("%s END\n", funcName);
    return 0;
}