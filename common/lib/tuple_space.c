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

    //displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);
    
    // receive ack
    unsigned char rec_packet[1024];
    memset(rec_packet, 0, sizeof(rec_packet));
    int total_packet_size_rec = receive_udp_packet_timeout(rec_packet, 1024, 1);
    int try_counter = 0;

    while (try_counter < (MAX_TS_OUT - 1) && total_packet_size_rec == -1)
    {
        send_udp_packet(packet, total_packet_size);
        total_packet_size_rec = receive_udp_packet_timeout(rec_packet, 1024, 1);
        try_counter++;
    }
    
    if (try_counter >= MAX_TS_OUT){
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    total_packet_size_rec = deserializePacket(rec_packet, &command, tuple_name_rec, fields, &num_fields_rec);
    //verify ack
    if(command != TS_CMD_OUT || my_strcmp(tuple_name, tuple_name_rec) || num_fields == 0){
        return TS_FAILURE;
    }

    return TS_SUCCESS;
}

/* Implementation of ts_inp function */
int ts_inp(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_IN_P, tuple_name, fields, num_fields);

    //displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    unsigned char rec_packet[1024];
    memset(rec_packet, 0, sizeof(rec_packet));

    int total_packet_size_rec = receive_udp_packet_timeout(rec_packet, 1024, 1);
    if (total_packet_size_rec == -1){
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    total_packet_size_rec = deserializePacket(rec_packet, &command, tuple_name_rec, fields, &num_fields_rec);
    if(total_packet_size_rec == 0) return TS_NO_TUPLE;

    return TS_SUCCESS;
}

/* Implementation of ts_rdp function */
int ts_rdp(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_RD_P, tuple_name, fields, num_fields);

    //displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    unsigned char rec_packet[1024];
    memset(rec_packet, 0, sizeof(rec_packet));

    int total_packet_size_rec = receive_udp_packet_timeout(rec_packet, 1024, 1);
    if (total_packet_size_rec == -1){
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    
    total_packet_size_rec = deserializePacket(rec_packet, &command, tuple_name_rec, fields, &num_fields_rec);
    if(total_packet_size_rec == 0) return TS_NO_TUPLE;
    

    return TS_SUCCESS;
}

/* Implementation of ts_in function */
int ts_in(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_IN, tuple_name, fields, num_fields);

    //displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    unsigned char rec_packet[1024];
    memset(rec_packet, 0, sizeof(rec_packet));

    int total_packet_size_rec = receive_udp_packet_timeout(rec_packet, 1024, 60*60);
    if (total_packet_size_rec == -1){
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    total_packet_size_rec = deserializePacket(rec_packet, &command, tuple_name_rec, fields, &num_fields_rec);
    if(total_packet_size_rec == 0) return TS_NO_TUPLE;

    return TS_SUCCESS;
}

/* Implementation of ts_rdp function */
int ts_rd(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size = serializePacket(packet, TS_CMD_RD, tuple_name, fields, num_fields);

    //displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    send_udp_packet(packet, total_packet_size);

    unsigned char rec_packet[1024];
    memset(rec_packet, 0, sizeof(rec_packet));

    int total_packet_size_rec = receive_udp_packet_timeout(rec_packet, 1024, 60*60);
    if (total_packet_size_rec == -1){
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    
    total_packet_size_rec = deserializePacket(rec_packet, &command, tuple_name_rec, fields, &num_fields_rec);
    if(total_packet_size_rec == 0) return TS_NO_TUPLE;
    

    return TS_SUCCESS;
}
