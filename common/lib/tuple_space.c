#include "tuple_space.h"
#include "tuple_protocol.h"
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

/* Implementation of ts_out function */
int ts_out(char* tuple_name, field_t* fields, int num_fields) {
    int total_packet_size = 0;
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    short flags[8] = {0};
    unsigned char flags_combined = 0x00;

    flags[0] = (TS_CMD_OUT >> 1) & 1;
    flags[1] = TS_CMD_OUT & 1;
    flags[2] = num_fields - 1;
    flags[3] = fields[0].is_actual;
    flags[4] = fields[0].type;
    if (num_fields > 1){
        flags[5] = fields[1].is_actual;
        flags[6] = fields[1].type;
    }

    for (int i = 0; i < 8; i++)
    {
        flags_combined |= (flags[i] & 1) << (7 - i);
    }
    
    packet[total_packet_size++] = flags_combined;
    
    packet[total_packet_size++] = strlen(tuple_name);
    
    for (int i = 0; i < 32; i++)
    {   
        if (i < strlen(tuple_name))
            packet[total_packet_size++] = tuple_name[i];
    }

    for (int i = 0; i < num_fields; i++)
    {
        if (fields[i].type == TS_INT){
            for (int j = 0; j<sizeof(int); j++){
                packet[total_packet_size++] = intToBytes(fields[i].data.int_field, j);
                //printf("%02x ", intToBytes(htonl(fields[0].data.int_field), i));
            }
        }
        else if (fields[i].type == TS_FLOAT){
            for (int j = 0; j<sizeof(int); j++){
                packet[total_packet_size++] = floatToBytes(fields[i].data.float_field, j);
                //printf("%02x ", floatToBytes(htonl(fields[1].data.float_field), i));
            }
        }
    }

    displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    return TS_SUCCESS;
}

/* Implementation of ts_inp function */
int ts_inp(char* tuple_name, field_t* fields, int num_fields) {
    // Implement the logic for ts_inp
    // Example pseudocode: 
    /*
    1. Check tuple_name, fields, and num_fields for validity
    2. Search for a matching tuple in the tuple space
    3. If found, populate fields with the retrieved tuple's data
    4. Return TS_SUCCESS if successful, otherwise TS_FAILURE
    */
    return TS_SUCCESS;
}

/* Implementation of ts_rdp function */
int ts_rdp(char* tuple_name, field_t* fields, int num_fields) {
    // Implement the logic for ts_rdp
    // Example pseudocode: 
    /*
    1. Check tuple_name, fields, and num_fields for validity
    2. Read a matching tuple from the tuple space without removing it
    3. If found, populate fields with the tuple's data
    4. Return TS_SUCCESS if successful, otherwise TS_FAILURE
    */
    return TS_SUCCESS;
}
