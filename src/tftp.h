#ifndef TFTP_H
#define TFTP_H

#include <netinet/udp.h>
#include "constante.h"

struct udphdr* udp;
unsigned char * tftp;


void print_udp();
void print_tftp_apli();
#endif
