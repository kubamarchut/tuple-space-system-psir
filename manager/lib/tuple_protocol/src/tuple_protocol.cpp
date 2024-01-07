#include "tuple_protocol.h"
#include <stdio.h>
#include <string.h>


int intToBytes(uint32_t num, int index){
    int pos = (sizeof(uint32_t) - 1) * 8;
    pos -= index * 8;

    return (num >> pos) & 0xFF;
}

int bytesToInt(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4) {
    int result = 0;

    result |= byte4;
    result <<= 8;
    result |= byte3;
    result <<= 8;
    result |= byte2;
    result <<= 8;
    result |= byte1;

    return result;
}

int floatToBytes(float number, int index){
    unsigned char* floatBytes = (unsigned char*)&number;

    printf("%d \n", sizeof(float) - index - 1);
    return floatBytes[sizeof(float) - index - 1];
}

float bytesToFloat(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4) {
    unsigned int combinedBytes = 0;

    combinedBytes |= byte4;
    combinedBytes <<= 8;
    combinedBytes |= byte3;
    combinedBytes <<= 8;
    combinedBytes |= byte2;
    combinedBytes <<= 8;
    combinedBytes |= byte1;

    float result;
    memcpy(&result, &combinedBytes, sizeof(float));

    return result;
}

int serializePacket(char* packet, int command, char* tuple_name, field_t* fields, int num_fields){
    int total_packet_size = 0;
    short flags[8] = {0};
    unsigned char flags_combined = 0x00;

    flags[0] = (command >> 1) & 1;
    flags[1] = command & 1;
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
        if(fields[i].is_actual == TS_YES){
            if (fields[i].type == TS_INT){
                for (int j = 0; j<sizeof(uint32_t); j++){
                    packet[total_packet_size++] = intToBytes(fields[i].data.int_field, j);
                }
            }
            else if (fields[i].type == TS_FLOAT){
                for (int j = 0; j<sizeof(float); j++){
                    packet[total_packet_size++] = floatToBytes(fields[i].data.float_field, j);
                }
            }
        }
    }

    return total_packet_size;
}

void displayProtocolBytes(unsigned char *packet, int total_packet_size, int tuple_name_len) {
    for (int i = 0; i < total_packet_size; i++) {
        printf("%02x ", packet[i]);

        if (i == 0 || i == 1 || i == tuple_name_len + 1 || i == tuple_name_len + 1 + 4)
            printf("| ");
    }
    printf("\n");
}
/* Implementation of ts_out function */
int ts_out(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_OUT, tuple_name, fields, num_fields);

    displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    Udp.write(packet, total_packet_size);

    return TS_SUCCESS;
}

/* Implementation of ts_inp function */
int ts_inp(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_IN, tuple_name, fields, num_fields);

    displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    unsigned char rec_packet[1024];
    memset(rec_packet, 0, sizeof(rec_packet));

    //send_udp_packet(rec_packet, total_packet_size);

    //int total_packet_size_rec = receive_udp_packet(packet, 1024);
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);


    return TS_SUCCESS;
}

/* Implementation of ts_rdp function */
int ts_rdp(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_RD, tuple_name, fields, num_fields);

    displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    //send_udp_packet(packet, total_packet_size);

    unsigned char rec_packet[1024];
    memset(rec_packet, 0, sizeof(rec_packet));

    //int total_packet_size_rec = receive_udp_packet(rec_packet, 1024);
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);

    

    return TS_SUCCESS;
}
