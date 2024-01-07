#ifndef TUPLE_PROTOCOL_H
#define TUPLE_PROTOCOL_H

#define TS_YES      1
#define TS_NO       0
#define TS_INT      0
#define TS_FLOAT    1
#define TS_SUCCESS  1
#define TS_FAILURE  0

#define TS_CMD_OUT  0
#define TS_CMD_IN   1
#define TS_CMD_RD   2

typedef struct {
    int is_actual;  /* does the data member contains data */
    int type;       /* what is the type of the data member */
    union {
        int int_field;
        float float_field;
        char* string_field;
    } data;
} field_t;          /* a new type corresponding to one field of a tuple*/

#define COMMAND_TYPE_MASK   0x03
#define COMMAND_TYPE_POS    (sizeof(char) * 8 - 2)
#define NUM_FIELDS_POS      (sizeof(char) * 8 - 3)
#define FIELD_1_ACTUAL_POS  (sizeof(char) * 8 - 4)
#define FIELD_1_TYPE_POS    (sizeof(char) * 8 - 5)
#define FIELD_2_ACTUAL_POS  (sizeof(char) * 8 - 6)
#define FIELD_2_TYPE_POS    (sizeof(char) * 8 - 7)

int serializePacket(char* packet, int command, char* tuple_name, field_t* fields, int num_fields);

void displayProtocolBytes(unsigned char *packet, int total_packet_size, int tuple_name_len);

int intToBytes(int number, int index);
int floatToBytes(float number, int index);

int bytesToInt(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);
float bytesToFloat(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);


#endif