#ifndef TUPLE_PROTOCOL_H
#define TUPLE_PROTOCOL_H

#define COMMAND_TYPE_MASK   0x03
#define COMMAND_TYPE_POS    (sizeof(char) * 8 - 2)
#define NUM_FIELDS_POS      (sizeof(char) * 8 - 3)
#define FIELD_1_ACTUAL_POS  (sizeof(char) * 8 - 4)
#define FIELD_1_TYPE_POS    (sizeof(char) * 8 - 5)
#define FIELD_2_ACTUAL_POS  (sizeof(char) * 8 - 6)
#define FIELD_2_TYPE_POS    (sizeof(char) * 8 - 7)

void displayProtocolBytes(unsigned char *packet, int total_packet_size, int tuple_name_len);

#endif