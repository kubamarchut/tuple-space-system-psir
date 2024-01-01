#include "udp_handler.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int udp_socket = -1; // Definition of the UDP socket
struct addrinfo *r = NULL;

int init_udp_socket(){
    struct addrinfo h;

    memset(&h, 0, sizeof(struct addrinfo));
    h.ai_family = PF_INET;
    h.ai_socktype = SOCK_DGRAM;
    h.ai_flags = AI_PASSIVE;

    if (getaddrinfo(CLIENT_IP, PORT, &h, &r) != 0)
    {
        printError(__LINE__, __FILE__, strerror(errno));
        exit(-1);
    }
    printTimestamp();
    printf(" - getaddrinfo successful\n");

    if ((udp_socket = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1)
    {
        printError(__LINE__, __FILE__, strerror(errno));
        exit(-1);
    }
    printTimestamp();
    printf(" - created new socket successfully\n");

    return udp_socket;
}

int send_udp_packet(char *buffer, int length){
    unsigned char message[length];

    snprintf(message, length, "%s", buffer);
    int pos = sendto(udp_socket,
                     message,
                     length,
                     0, r->ai_addr, r->ai_addrlen);
    if (pos < 0)
    {
        printError(__LINE__, __FILE__, strerror(errno));
        exit(-1);
    }
    else
    {
        printTimestamp();
        printf(" - sending a message\n");
    }
    return pos;
}

int receive_udp_packet(char *buffer, int length){
    struct sockaddr_in c;
    int pos, c_len = sizeof(c);
    if ((pos = recvfrom(s, buffer, length, 0,
                        (struct sockaddr *)&c, &c_len)) < 0
                       ){
        printError(__LINE__, __FILE__, strerror(errno));
        exit(-1);
    }
    received_command[pos] = '\0';
    return pos; // Placeholder, replace with actual logic
}

void close_udp_socket()
{
    if (udp_socket != -1)
    {
        close(udp_socket);
        udp_socket = -1;
    }
}
