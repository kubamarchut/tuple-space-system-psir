#include "tuple_space.h"
#include "tuple_protocol.h"
#include "udp_setup.h"

int ts_out(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_OUT, tuple_name, fields, num_fields);

    //displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    
    send_udp_packet(packet, total_packet_size);

    return TS_SUCCESS;
}

/* Implementation of ts_inp function */
int ts_inp(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_IN, tuple_name, fields, num_fields);

    //displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    memset(packet, 0, sizeof(packet));
    int total_packet_size_rec = receive_udp_packet(packet, 1024);
    
    //printf("received: ");
    //displayProtocolBytes(packet, total_packet_size_rec, packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    total_packet_size_rec = deserializePacket(packet, &command, tuple_name_rec, fields, &num_fields_rec);

    return TS_SUCCESS;
}

/* Implementation of ts_rdp function */
int ts_rdp(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_RD, tuple_name, fields, num_fields);

    //displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    memset(packet, 0, sizeof(packet));
    
    int total_packet_size_rec = receive_udp_packet(packet, 1024);
    
    //displayProtocolBytes(packet, total_packet_size_rec, packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    total_packet_size_rec = deserializePacket(packet, &command, tuple_name_rec, fields, &num_fields_rec);

    return TS_SUCCESS;
}