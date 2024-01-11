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
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_OUT, tuple_name, fields, num_fields);

    displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    return TS_SUCCESS;
}

/* Implementation of ts_inp function */
int ts_inp(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_IN, tuple_name, fields, num_fields);

    displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    unsigned char rec_packet[1024];
    memset(rec_packet, 0, sizeof(rec_packet));

    int total_packet_size_rec = receive_udp_packet_timeout(rec_packet, 1024, 1);
    if (total_packet_size_rec == -1){
        return TS_FAILURE;
    }
    
    displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    total_packet_size_rec = deserializePacket(rec_packet, &command, tuple_name_rec, fields, &num_fields_rec);

    return TS_SUCCESS;
}

/* Implementation of ts_rdp function */
int ts_rdp(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_RD, tuple_name, fields, num_fields);

    displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    unsigned char rec_packet[1024];
    memset(rec_packet, 0, sizeof(rec_packet));

    int total_packet_size_rec = receive_udp_packet_timeout(rec_packet, 1024, 1);
    if (total_packet_size_rec == -1){
        return TS_FAILURE;
    }
    
    displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    
    total_packet_size_rec = deserializePacket(rec_packet, &command, tuple_name_rec, fields, &num_fields_rec);
    

    return TS_SUCCESS;
}
