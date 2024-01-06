#ifndef UDP_HANDLER_H
#define UDP_HANDLER_H

extern int udp_socket; // Declaration of the UDP socket
extern struct addrinfo *r;

int init_udp_socket(char* ip_addr, char* port);

//handling simple UDP packet sending
int send_udp_packet(unsigned char* buffer, int length);
int receive_udp_packet(char* buffer, int length);
void close_udp_socket();

#endif /* UDP_HANDLER_H */
