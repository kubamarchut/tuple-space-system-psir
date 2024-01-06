#include <stdio.h>

void displayProtocolBytes(unsigned char *packet, int total_packet_size, int tuple_name_len) {
    for (int i = 0; i < total_packet_size; i++) {
        printf("%02x ", packet[i]);

        if (i == 0 | i == 1 || i == tuple_name_len + 1 || i == tuple_name_len + 1 + 4)
            printf("| ");
    }
    printf("\n");
}