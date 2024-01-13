#ifndef TUPLE_PROTOCOL_H
#define TUPLE_PROTOCOL_H

#define COMMAND_TYPE_MASK   0x07
#define COMMAND_TYPE_POS    (sizeof(char) * 8 - 3)
#define NUM_FIELDS_POS      (sizeof(char) * 8 - 3)
#define FIELD_1_ACTUAL_POS  (sizeof(char) * 8 - 4)
#define FIELD_1_TYPE_POS    (sizeof(char) * 8 - 5)
#define FIELD_2_ACTUAL_POS  (sizeof(char) * 8 - 6)
#define FIELD_2_TYPE_POS    (sizeof(char) * 8 - 7)

int my_strcmp(const char *str1, const char *str2);
int genChecksum(unsigned char *packet, int total_packet_size, int bit_len);
int getBit(unsigned char byte, int position);

int serializePacket(char* packet, int command, char* tuple_name, field_t* fields, int num_fields);
int deserializePacket(char* packet, int* command, char* tuple_name, field_t* fields, int* num_fields);

void displayProtocolBytes(unsigned char *packet, int total_packet_size, int tuple_name_len);

int intToBytes(int number, int index);
int floatToBytes(float number, int index);

int bytesToInt(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);
float bytesToFloat(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);

#endif