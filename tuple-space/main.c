#include "../common/lib/udp_handler.h"
#include "../common/lib/utilities.h"
#include "../common/lib/tuple_space.h"
#include "../common/lib/tuple_protocol.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define PORT "5000"
#define MAX_BUFF 1024

int getBit(unsigned char byte, int position) {
    return (byte >> position) & 1;
}

int main(){
    int socket = init_udp_socket("", PORT);


    fd_set readfds;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = (1 * 5000);
    int sel;
    int max_sd = socket;

    for (;;)
    {
        FD_ZERO(&readfds);
        FD_SET(socket, &readfds);

        if ((sel = select(max_sd + 1, &readfds, NULL, NULL, &tv)) < 0)
        {
            printError(__LINE__, __FILE__, strerror(errno));
        }
        else if (sel > 0)
        {
            if (FD_ISSET(socket, &readfds)) {
                struct sockaddr_in c;
                int c_len = sizeof(c);

                unsigned char received_message[MAX_BUFF];

                int pos=recvfrom(socket, received_message, MAX_BUFF, 0, (struct sockaddr*)&c, &c_len);
                if(pos<0){
                    printError(__LINE__, __FILE__, strerror(errno));
                    exit(-1);
                }
                received_message[pos]='\0';
                printTimestamp();
                printf(" - new message from (%s:%d)\n", inet_ntoa(c.sin_addr),ntohs(c.sin_port));

                int tuple_name_len = received_message[1];
                displayProtocolBytes(received_message, pos, tuple_name_len);

                int command_type = received_message[0] >> COMMAND_TYPE_POS & COMMAND_TYPE_MASK;
                int num_fields = getBit(received_message[0], NUM_FIELDS_POS);

                printf("command type: %d\n", command_type);
                printf("tuple size: %d\n", num_fields);

                unsigned char tuple_name[tuple_name_len];
                for (int i = 0; i < tuple_name_len; i++)
                {
                    tuple_name[i] = received_message[i+1];
                }
                tuple_name[tuple_name_len] = '\0';
                
                printf("tuple name: %s\n", tuple_name);
                
                int bit_pos = 4;
                field_t received_tuple[num_fields+1];
                for (int i = 0; i <= num_fields; i++)
                {
                    received_tuple[i].is_actual = received_message[0] >> (sizeof(char) * 8 - bit_pos++) & 1;
                    received_tuple[i].type = received_message[0] >> (sizeof(char) * 8 - bit_pos++) & 1;
                }
                int ptr_pos = tuple_name_len+2;
                for (int i = 0; i <= num_fields; i++)
                {
                    if (received_tuple[i].is_actual == TS_YES)
                    {   
                        if (received_tuple[i].type == TS_INT){
                            received_tuple[i].data.int_field = bytesToInt(received_message[ptr_pos++], 
                                                    received_message[ptr_pos++], 
                                                    received_message[ptr_pos++], 
                                                    received_message[ptr_pos++]);
                            printf("field 1: %d\n", received_tuple[i].data.int_field);
                        }
                        else if (received_tuple[i].type == TS_FLOAT){
                            received_tuple[i].data.float_field = bytesToFloat(received_message[ptr_pos++], 
                                                        received_message[ptr_pos++], 
                                                        received_message[ptr_pos++], 
                                                        received_message[ptr_pos++]);
                            printf("field 2: %f\n", received_tuple[i].data.float_field);
                        }
                    }
                }
            }
        }
    }


    return 0;
}
