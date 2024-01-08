#ifndef UDP_SETUP_H
#define UDP_SETUP_H

#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>

extern ZsutEthernetUDP Udp;

void setupUDP();
int send_udp_packet(unsigned char *buffer, int length);
int receive_udp_packet(char *buffer, int length);

#endif
