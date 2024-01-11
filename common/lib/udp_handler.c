#include "udp_handler.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>

int udp_socket = -1; // Definition of the UDP socket
int server = 0;
struct addrinfo *r = NULL;

int init_udp_socket(char *ip_address, char *port){
    struct addrinfo h;

    memset(&h, 0, sizeof(struct addrinfo));
    h.ai_family = PF_INET;
    h.ai_socktype = SOCK_DGRAM;
    h.ai_flags = AI_PASSIVE;

    if (strlen(ip_address) == 0) ip_address = NULL;

    if (getaddrinfo(ip_address, port, &h, &r) != 0)
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

    if (ip_address == NULL){
        server = 1;
        if (bind(udp_socket, r->ai_addr, r->ai_addrlen) != 0)
        {
            printError(__LINE__, __FILE__, strerror(errno));
            exit(-1);
        }
        printTimestamp();
        printf(" - socket bound to address successfully\n");
    }

    return udp_socket;
}

int send_udp_packet(unsigned char *buffer, int length){
    unsigned char message[length];

    //snprintf(message, length, "%s", buffer);
    int pos = sendto(udp_socket,
                     buffer,
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
    if ((pos = recvfrom(udp_socket, buffer, length, 0,
                        (struct sockaddr *)&c, &c_len)) < 0
                       ){
        printError(__LINE__, __FILE__, strerror(errno));
        exit(-1);
    }
    else
    {
        printTimestamp();
        printf(" - receiving a message\n");
    }
    buffer[pos] = '\0';
    return pos; // Placeholder, replace with actual logic
}

int receive_udp_packet_timeout(char *buffer, int length, int timeout_seconds) {
    struct sockaddr_in c;
    int pos, c_len = sizeof(c);

    // Set up fd_set for select
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(udp_socket, &read_fds);

    // Set timeout for select
    struct timeval timeout;
    timeout.tv_sec = timeout_seconds;
    timeout.tv_usec = 0;

    // Use select to wait for data or timeout
    int ready = select(udp_socket + 1, &read_fds, NULL, NULL, &timeout);

    if (ready < 0) {
        printError(__LINE__, __FILE__, strerror(errno));
        exit(-1);
    } else if (ready == 0) {
        // Timeout occurred
        printf("Timeout occurred. No data received.\n");
        return -1;
    }

    // Data is ready to be received
    if ((pos = recvfrom(udp_socket, buffer, length, 0,
                        (struct sockaddr *)&c, &c_len)) < 0) {
        printError(__LINE__, __FILE__, strerror(errno));
        exit(-1);
    } else {
        printTimestamp();
        printf(" - receiving a message\n");
    }

    buffer[pos] = '\0';
    return pos;
}

void close_udp_socket()
{
    if (udp_socket != -1)
    {
        close(udp_socket);
        udp_socket = -1;
    }
}
