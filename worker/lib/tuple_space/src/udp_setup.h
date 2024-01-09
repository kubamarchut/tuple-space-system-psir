#ifndef UDP_SETUP_H
#define UDP_SETUP_H

#include <ZsutEthernet.h>
#include <ZsutEthernetUdp.h>

// declaring the Udp object
extern ZsutEthernetUDP Udp;

// function to set up the UDP socket
// returns:
//  1 if socket created successfully. 0 if no socket available
int setupUDP();

// function to send a UDP packet
// parameters:
//   buffer: pointer to the data to be sent
//   length: number of bytes to be sent
// returns:
//   1 if the packet was successfully sent, 0 if there was an error
int send_udp_packet(unsigned char *buffer, int length);

// function to receive a UDP packet
// parameters:
//   buffer: pointer to store the received data
//   length: maximum length of the buffer
// returns:
//   size of the received packet if successful, -1 if there was an error in receiving or reading the packet
int receive_udp_packet(char *buffer, int length);

#endif
