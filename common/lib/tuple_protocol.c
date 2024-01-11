#include <stdio.h>
#include <string.h>
#include "tuple_space.h"
#include "tuple_protocol.h"

int powerOfTwo(int n){
    if(n<0){
        return -1;
    }
    int result = 1;
    for (int i = 0; i < n; i++)
    {
        result *= 2;
    }
    return result;
}

int genChecksum(unsigned char *packet, int total_packet_size, int bit_len){
    int sum = 0;
    for (int i = 0; i < total_packet_size; i++)
    {
        sum += packet[i];
    }
    
    return sum % powerOfTwo(bit_len);
}

int intToBytes(int num, int index){
    int pos = (sizeof(int) - 1) * 8;
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
    unsigned int* ptr = (unsigned int*)&number;
    unsigned int num = *ptr;

    int pos = (sizeof(int) - 1) * 8;
    pos -= index * 8;

    return (num >> pos) & 0xff;
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
    if (strlen(tuple_name) < 1){
        return -1;
    }
    if (strlen(tuple_name) > 32){
        return -1;
    }
    if (num_fields < 1){
        return -1;
    }
    if (num_fields > 16){
        return -1;
    }
    num_fields--;
    
    int total_packet_size = 0;
    short flags[16] = {0};
    unsigned char flags_combined = 0x00;

    flags[0] = (command >> 2) & 1;
    flags[1] = (command >> 1) & 1;
    flags[2] = command & 1;
    //flags[3] = ; direction
    flags[4] = (num_fields >> 3) & 1;
    flags[5] = (num_fields >> 2) & 1;
    flags[6] = (num_fields >> 1) & 1;
    flags[7] = num_fields & 1;

    for (int i = 0; i < 8; i++)
    {
        flags_combined |= (flags[i] & 1) << (7 - i);
    }
    
    packet[total_packet_size++] = flags_combined;
    
    packet[total_packet_size++] = strlen(tuple_name) - 1;
    
    for (int i = 0; i < 32; i++)
    {   
        if (i < strlen(tuple_name))
            packet[total_packet_size++] = tuple_name[i];
    }

    for (int i = 0; i <= num_fields; i++)
    {
        unsigned char tuple_desc = 0x00;
        tuple_desc |= (fields[i].is_actual) << 7;
        tuple_desc |= (fields[i].type) << 5;
        if (fields[i].type == TS_INT){
            tuple_desc |= (sizeof(fields[i].data.int_field) & 0x1f);
            packet[total_packet_size++] = tuple_desc;
            for (int j = 0; j<sizeof(int); j++){
                packet[total_packet_size++] = intToBytes(fields[i].data.int_field, j);
                //printf("%02x ", intToBytes(htonl(fields[0].data.int_field), i));
            }
        }
        else if (fields[i].type == TS_FLOAT){
            tuple_desc |= (sizeof(fields[i].data.float_field) & 0x1f);
            packet[total_packet_size++] = tuple_desc;
            for (int j = 0; j<sizeof(int); j++){
                packet[total_packet_size++] = floatToBytes(fields[i].data.float_field, j);
                //printf("%02x ", floatToBytes(htonl(fields[1].data.float_field), i));
            }
        }
        else if (fields[i].type == TS_STR){
            tuple_desc |= (strlen(fields[i].data.string_field) & 0x1f);
            packet[total_packet_size++] = tuple_desc;
            for (int j = 0; j < strlen(fields[i].data.string_field); j++)
            {
                packet[total_packet_size++] = fields[i].data.string_field[j];
            }
            

        }
    }
    int checksum = genChecksum(packet, total_packet_size, 3);
    packet[1] |= (checksum << 5);

    return total_packet_size;
}

int getBit(unsigned char byte, int position) {
    return (byte >> position) & 1;
}

int deserializePacket(char* packet, int* command, char* tuple_name, field_t* fields, int* num_fields) {
    int total_packet_size = 0;

    // Extract flags_combined byte
    unsigned char flags_combined = packet[total_packet_size++];
    // Extract tuple_name length
    int tuple_name_length = (packet[total_packet_size++] & 0x1f) + 1;
    // Extract tuple_name
    for (int i = 0; i < tuple_name_length; ++i) {
        tuple_name[i] = packet[total_packet_size++];
    }
    tuple_name[tuple_name_length] = '\0'; // Null-terminate the string


    // Reconstruct command and num_fields from flags_combined
    *command = packet[0] >> COMMAND_TYPE_POS & COMMAND_TYPE_MASK;
    *num_fields = (packet[0] & 0x0f) + 1;

    // Extract fields
    for (int i = 0; i < *num_fields; i++)
    {
        unsigned char field_desc = packet[total_packet_size++];
        fields[i].is_actual = field_desc >> (sizeof(char) * 8 - 1) & 1;
        fields[i].type = field_desc >> (sizeof(char) * 8 - 3) & 0x03;
        int field_len = (field_desc & 0x1f) + 1;

        if (fields[i].is_actual == TS_YES) {
            if (fields[i].type == TS_INT) {
                // Assuming sizeof(int) is 4 bytes
                fields[i].data.int_field = bytesToInt(
                    packet[total_packet_size++],
                    packet[total_packet_size++],
                    packet[total_packet_size++],
                    packet[total_packet_size++]
                );
            } else if (fields[i].type == TS_FLOAT) {
                // Assuming sizeof(float) is 4 bytes
                fields[i].data.float_field = bytesToFloat(packet[total_packet_size++], 
                                                            packet[total_packet_size++], 
                                                            packet[total_packet_size++], 
                                                            packet[total_packet_size++]);
            }
            else if(fields[i].type == TS_STR) {
                // Assuming sizeof(float) is 4 bytes
                for (int j = 0; j < field_len; j++)
                {
                    fields[i].data.string_field[j] = packet[total_packet_size++];
                    printf("field str: %c\n", fields[i].data.string_field[j]);
                }
            }
        }
    }

    //return 0;
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
