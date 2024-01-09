#include "../common/lib/udp_handler.h"
#include "../common/lib/utilities.h"
#include "../common/lib/tuple_space.h"
#include "../common/lib/tuple_protocol.h"
#include "./lib/tuple_utils.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PORT "5000"
#define MAX_BUFF 1024
#define MAX_TUPLES 512

int stats[4] = {0,0,0,0};

tuple_struct tuples[MAX_TUPLES], empty_tuple;

int main(){
    int udp_socket = init_udp_socket("", PORT);

    int tuples_count = 0;
    
    fd_set readfds;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = (1 * 5000);
    int sel;
    int max_sd = udp_socket;

    for (;;)
    {
        FD_ZERO(&readfds);
        FD_SET(udp_socket, &readfds);

        if ((sel = select(max_sd + 1, &readfds, NULL, NULL, &tv)) < 0)
        {
            printError(__LINE__, __FILE__, strerror(errno));
        }
        else if (sel > 0)
        {
            if (FD_ISSET(udp_socket, &readfds)) {
                struct sockaddr_in c;
                int c_len = sizeof(c);

                unsigned char received_message[MAX_BUFF];

                int pos=recvfrom(udp_socket, received_message, MAX_BUFF, 0, (struct sockaddr*)&c, &c_len);
                if(pos<0){
                    printError(__LINE__, __FILE__, strerror(errno));
                    exit(-1);
                }
                received_message[pos]='\0';
                //printTimestamp();
                //printf(" - new message from (%s:%d)\n", inet_ntoa(c.sin_addr),ntohs(c.sin_port));
                
                int command_type;
                unsigned char tuple_name[32];
                int num_fields;
                field_t tuple_fields[2];

                int total_packet_size = deserializePacket(received_message,
                                                        &command_type, tuple_name,
                                                        tuples[tuples_count].fields,
                                                        &num_fields);

                if(command_type == TS_CMD_OUT) {
                    strcpy(tuples[tuples_count].tuple_name, tuple_name);
                    tuples[tuples_count].number_of_fields = num_fields;
                    stats[TS_CMD_OUT]++;
                    tuples_count++;
                }
                else if (command_type == TS_CMD_RD || command_type == TS_CMD_IN){
                    if (command_type == TS_CMD_RD) stats[TS_CMD_RD]++;
                    int indexOfFound = searchTupleByName(tuples, tuple_name, MAX_TUPLES);

                    unsigned char packet[1024];
                    if (indexOfFound >= 0){
                        int total_packet_size = serializePacket(packet, TS_CMD_RD, tuples[indexOfFound].tuple_name, 
                               tuples[indexOfFound].fields,
                               tuples[indexOfFound].number_of_fields);
                        sendto(udp_socket, packet, total_packet_size, 0, (struct sockaddr*)&c, c_len);
                        if (command_type == TS_CMD_IN){
                            stats[TS_CMD_IN]++;
                            tuples_count = removeTupleByID(tuples, indexOfFound, tuples_count, empty_tuple);
                        }
                    }
                    else{
                        field_t empty_tuple[1];
                        int total_packet_size = serializePacket(packet, TS_CMD_NO_TUPLE, tuple_name, 
                               empty_tuple, 0);
                        sendto(udp_socket, packet, pos, 0, (struct sockaddr*)&c, c_len);
                    }
                }
                else if (command_type == TS_CMD_IN){
                    
                }
                printTupleArray(tuples, 10, stats);
            }
        }
    }


    return 0;
}
