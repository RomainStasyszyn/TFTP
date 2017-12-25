#ifndef PROJET_H
#define PROJET_H

#include "constante.h"
#include "ethernet.h"
#include "ip.h"
#include "tftp.h"
#include "bootp.h"
#include "paquet.h"

void got_packet(u_char *args, const struct pcap_pkthdr *hdr, const u_char *truepacket);
void parser(const u_char* truepacket);
void aff(const struct pcap_pkthdr *hdr);

#endif
