#ifndef TUPLE_UTILS_H
#define TUPLE_UTILIS_H

#include "../../common/lib/tuple_space.h"
#include <arpa/inet.h>

#define MAX_TUPLES 128


typedef struct {
    char tuple_name[32];
    field_t fields[16];
    int number_of_fields;
} tuple_struct;

typedef struct
{
    struct sockaddr_in source;
    int command;
    tuple_struct tuple;

} tuple_ask;

extern int stats[6];
extern tuple_struct tuples[MAX_TUPLES], empty_tuple;
extern tuple_ask tuple_asks[MAX_TUPLES], empty_ask;

void copyFieldArrayToTuple(tuple_struct *destTuple, field_t *fieldArray, int numFields);

int addTupleToSpace(int *tuples_count, char *tuple_name, int num_fields, field_t *tuple_fields, int max_tuples);
int removeTupleByID(tuple_struct tuples[], int index, int tuples_count, tuple_struct empty);
int removeTupleAskByID(tuple_ask tuple_asks[], int index, int tuple_asks_count, tuple_ask empty);
int searchMatchingTupleAsk(tuple_ask tuple_asks[], int *tuples_asks_count, char* search_name, int search_num_fields, field_t* search_fields, int max_index);
void printTupleArray(tuple_struct tuples[], int size, int stats[]);

int searchMatchingTuple(tuple_struct tuples[], int *tuples_count, char* search_name, int search_num_fields, field_t* search_fields, int max_index);

#endif