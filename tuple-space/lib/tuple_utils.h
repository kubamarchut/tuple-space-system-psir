#ifndef TUPLE_UTILS_H
#define TUPLE_UTILIS_H

#include "../../common/lib/tuple_space.h"

typedef struct {
    char tuple_name[32];
    field_t fields[2];
    int number_of_fields;
} tuple_struct;

int removeTupleByID(tuple_struct tuples[], int index, int tuples_count, tuple_struct empty);
int searchTupleByName(tuple_struct tuples[], char* search_name, int max_index);
void printTupleArray(tuple_struct tuples[], int size, int stats[]);

#endif