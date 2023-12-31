#ifndef UDP_HANDLER_H
#define UDP_HANDLER_H

extern int udp_socket; // Declaration of the UDP socket

int init_udp_socket();
int send_udp_packet(char* buffer, int length);
int receive_udp_packet(char* buffer, int length);
void close_udp_socket();

#endif /* UDP_HANDLER_H */
