#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <arpa/inet.h>

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    printf("Captured a packet with length of [%d]\n", pkthdr->len);
    // Additional processing on 'packet' can be done here
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

    // Capture packets in an infinite loop (can set to specific number if desired)
    pcap_loop(handle, 10, packet_handler, NULL); // Capture 10 packets

    // Close the handle
    pcap_close(handle);

    return 0;
}

int main(int argc, char** argv)
{
    const char funcName [] = "main - ";
    printf("%s start\n", funcName);

    printf("%s end\n", funcName);
    return 0;
}