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
#define MAX_TUPLES 512

typedef struct {
    char tuple_name[32];
    field_t fields[2];
    int number_of_fields;
} tuple_struct;

tuple_struct tuples[MAX_TUPLES];

int searchTupleByName(char* search_name) {
    for (int i = 0; i < MAX_TUPLES; ++i) {
        if (strcmp(tuples[i].tuple_name, search_name) == 0) {
            return i; // Found the tuple, return its index
        }
    }
    return -1; // Tuple with the specified name not found
}

void printTupleArray(tuple_struct tuples[], int size) {

    printf("+--------------------+---------------+--------------------+--------------------+\n");
    printf("|     Tuple Name     | No. of Fields |      Field 1       |      Field 2       |\n");
    printf("+--------------------+---------------+--------------------+--------------------+\n");
    for (int i = 0; i < size; ++i) {
        if (strlen(tuples[i].tuple_name) > 0){
            printf("| %*s | %*d |", 18, tuples[i].tuple_name, 13, tuples[i].number_of_fields);
        }
        else{
            printf("| %*c | %*d |", 18, '-', 13, tuples[i].number_of_fields);
        }
        
        
        for (int j = 0; j < 2; ++j) {
            if (j < tuples[i].number_of_fields) {
                if (tuples[i].fields[j].is_actual) {
                    if (tuples[i].fields[j].type == 0) {
                        printf(" Int: %13d ", tuples[i].fields[j].data.int_field);
                    } else if (tuples[i].fields[j].type == 1) {
                        printf(" Float: %11.2f ", tuples[i].fields[j].data.float_field);
                    } else if (tuples[i].fields[j].type == 2) {
                        printf(" String: %s ", tuples[i].fields[j].data.string_field);
                    }
                }
                else {
                    if (tuples[i].fields[j].type == 0) {
                        printf(" Int: %13c ", '?');
                    } else if (tuples[i].fields[j].type == 1) {
                        printf(" Float: %11c ", '?');
                    } else if (tuples[i].fields[j].type == 2) {
                        printf(" String: %c ", '?');
                    }
                }
                printf("|");
            } else {
                printf(" %*s |", 18, "-"); // Placeholder for empty field
            }
        }
        printf("\n");
    }
    printf("+--------------------+---------------+--------------------+--------------------+\n");
}

int getBit(unsigned char byte, int position) {
    return (byte >> position) & 1;
}

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
                printTimestamp();
                printf(" - new message from (%s:%d)\n", inet_ntoa(c.sin_addr),ntohs(c.sin_port));

                int tuple_name_len = received_message[1];
                //displayProtocolBytes(received_message, pos, tuple_name_len);

                int command_type = received_message[0] >> COMMAND_TYPE_POS & COMMAND_TYPE_MASK;
                int num_fields = getBit(received_message[0], NUM_FIELDS_POS);

                printf("command type: %d\n", command_type);
                //printf("tuple size: %d\n", num_fields);

                unsigned char tuple_name[tuple_name_len];
                for (int i = 0; i < tuple_name_len; i++)
                {
                    tuple_name[i] = received_message[i+2];
                }
                tuple_name[tuple_name_len] = '\0';
                
                //printf("tuple name: %s\n", tuple_name);
                
                int bit_pos = 4;
                for (int i = 0; i <= num_fields; i++)
                {
                    tuples[tuples_count].fields[i].is_actual = received_message[0] >> (sizeof(char) * 8 - bit_pos++) & 1;
                    tuples[tuples_count].fields[i].type = received_message[0] >> (sizeof(char) * 8 - bit_pos++) & 1;
                }
                int ptr_pos = tuple_name_len+2;
                for (int i = 0; i <= num_fields; i++)
                {
                    if (tuples[0].fields[i].is_actual == TS_YES)
                    {   
                        if (tuples[0].fields[i].type == TS_INT){
                            tuples[tuples_count].fields[i].data.int_field = bytesToInt(received_message[ptr_pos++], 
                                                    received_message[ptr_pos++], 
                                                    received_message[ptr_pos++], 
                                                    received_message[ptr_pos++]);
                            //printf("field 1: %d\n", tuples[tuples_count].fields[i].data.int_field);
                        }
                        else if (tuples[0].fields[i].type == TS_FLOAT){
                            tuples[tuples_count].fields[i].data.float_field = bytesToFloat(received_message[ptr_pos++], 
                                                        received_message[ptr_pos++], 
                                                        received_message[ptr_pos++], 
                                                        received_message[ptr_pos++]);
                            //printf("field 2: %f\n", tuples[tuples_count].fields[i].data.float_field);
                        }
                    }
                }
                strcpy(tuples[tuples_count].tuple_name, tuple_name);
                tuples[tuples_count].number_of_fields = num_fields+1;

                printTupleArray(tuples, 10);
                tuples_count++;

                if (command_type == TS_CMD_RD){
                    int indexOfFound = searchTupleByName(tuple_name);
                    printf("Found tuple index: %d \n", indexOfFound);

                    if (indexOfFound >= 0){
                        unsigned char packet[1024];
                        int total_packet_size = createPacket(packet, TS_CMD_RD, tuples[indexOfFound].tuple_name, 
                               tuples[indexOfFound].fields,
                               tuples[indexOfFound].number_of_fields);
                        sendto(udp_socket, packet, total_packet_size, 0, (struct sockaddr*)&c, c_len);
                    }
                }

            }
        }
    }


    return 0;
}
