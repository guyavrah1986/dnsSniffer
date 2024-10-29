#pragma once

#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

int parseDnsPacket(unsigned char *user, const struct pcap_pkthdr *pkthdr, const unsigned char *packet);
void packet_handler(unsigned char *user_data, const struct pcap_pkthdr *pkthdr, const unsigned *packet);
int sampleFuncUsingLibpcap();