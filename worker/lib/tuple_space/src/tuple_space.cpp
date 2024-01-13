#include "tuple_space.h"
#include "tuple_protocol.h"
#include "udp_setup.h"

int ts_out(char* tuple_name, field_t* fields, int num_fields) {
    unsigned char packet[100], rec_packet[100];
    memset(packet, 0, sizeof(packet));
    memset(rec_packet, 0, sizeof(rec_packet));

    int total_packet_size = serializePacket(packet, TS_CMD_OUT, tuple_name, fields, num_fields);

    //displayProtocolBytes(packet, total_packet_size, strlen(tuple_name));
    
    
    send_udp_packet(packet, total_packet_size);

    // receive ack
    int total_packet_size_rec = -1;

    for (int attempt = 0; attempt < 3; ++attempt) {
        total_packet_size_rec = receive_udp_packet(packet, 1024);

        if (total_packet_size_rec != -1) {
            // Successful reception, break out of the loop
            break;
        }
        delay(100);
    }

    if (total_packet_size_rec == -1) {
        // All attempts failed
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    total_packet_size_rec = deserializePacket(rec_packet, &command, tuple_name_rec, fields, &num_fields_rec);
    //verify ack
    if(command != TS_CMD_OUT || my_strcmp(tuple_name, tuple_name_rec) == 0){
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

    //unsigned char rec_packet[1024];
    memset(packet, 0, sizeof(packet));

    int total_packet_size_rec = -1;

    for (int attempt = 0; attempt < 3; ++attempt) {
        total_packet_size_rec = receive_udp_packet(packet, 1024);

        if (total_packet_size_rec != -1) {
            // Successful reception, break out of the loop
            break;
        }
        delay(100);
    }

    if (total_packet_size_rec == -1) {
        // All attempts failed
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(packet, total_packet_size_rec, 43);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    total_packet_size_rec = deserializePacket(packet, &command, tuple_name_rec, fields, &num_fields_rec);
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

    memset(packet, 0, sizeof(packet));

    // receive ack
    int total_packet_size_rec = -1;

    for (int attempt = 0; attempt < 3; ++attempt) {
        total_packet_size_rec = receive_udp_packet(packet, 1024);

        if (total_packet_size_rec != -1) {
            // Successful reception, break out of the loop
            break;
        }
        delay(100);
    }

    if (total_packet_size_rec == -1) {
        // All attempts failed
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    
    total_packet_size_rec = deserializePacket(packet, &command, tuple_name_rec, fields, &num_fields_rec);
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

    memset(packet, 0, sizeof(packet));

    // receive ack
    int total_packet_size_rec = -1;

    for (int attempt = 0; attempt < 3; ++attempt) {
        total_packet_size_rec = receive_udp_packet(packet, 1024);

        if (total_packet_size_rec != -1) {
            // Successful reception, break out of the loop
            break;
        }
        delay(100);
    }

    if (total_packet_size_rec == -1) {
        // All attempts failed
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(packet, total_packet_size_rec, packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    total_packet_size_rec = deserializePacket(packet, &command, tuple_name_rec, fields, &num_fields_rec);
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

    memset(packet, 0, sizeof(packet));
    
    // receive ack
    int total_packet_size_rec = -1;

    for (int attempt = 0; attempt < 3; ++attempt) {
        total_packet_size_rec = receive_udp_packet(packet, 1024);

        if (total_packet_size_rec != -1) {
            // Successful reception, break out of the loop
            break;
        }
        delay(100);
    }

    if (total_packet_size_rec == -1) {
        // All attempts failed
        return TS_FAILURE;
    }
    
    //displayProtocolBytes(rec_packet, total_packet_size_rec, rec_packet[1]);
    int command;
    unsigned char tuple_name_rec[32];
    int num_fields_rec;
    
    total_packet_size_rec = deserializePacket(packet, &command, tuple_name_rec, fields, &num_fields_rec);
    if(total_packet_size_rec == 0) return TS_NO_TUPLE;
    

    return TS_SUCCESS;
}
