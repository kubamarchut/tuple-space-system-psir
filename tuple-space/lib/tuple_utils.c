#include "tuple_utils.h"
#include "../../common/lib/tuple_space.h"
#include "../../common/lib/tuple_protocol.h"

#include <stdio.h> //printf
#include <string.h> //strlen


int stats[8] = {0};
tuple_struct tuples[MAX_TUPLES], empty_tuple;
tuple_ask tuple_asks[MAX_TUPLES], empty_ask;

const char *stats_labels[8] = {
        "\"out\" messages",
        "\"in\" messages",
        "\"rd\" messages",
        "\"inp\" messages",
        "\"rdp\" messages",
        "No matching tuples",
        "Total received",
        "Avg. message length"
    };

int compareTuples(tuple_struct tupleA, char* search_name, int search_num_fields, field_t* search_fields){
    if (my_strcmp(tupleA.tuple_name, search_name) == 0) {
        // comparing fields
        if (tupleA.number_of_fields == search_num_fields){
            int fields_match = 1;  // Assume fields are matching until proven otherwise
            for (int j = 0; j < search_num_fields; j++) {
                if (tupleA.fields[j].type != search_fields[j].type){
                    fields_match = 0;  // Fields do not match
                    break;
                }
                else {
                    if (search_fields[j].type == TS_INT){
                        if (search_fields[j].is_actual == tupleA.fields[j].is_actual &&
                        search_fields[j].data.int_field == tupleA.fields[j].data.int_field){
                            fields_match = 0;  // Fields do not match
                            break;
                        }
                    }
                    else if (search_fields[j].type == TS_FLOAT){
                        if (search_fields[j].is_actual == tupleA.fields[j].is_actual &&
                        search_fields[j].data.float_field == tupleA.fields[j].data.float_field){
                            fields_match = 0;  // Fields do not match
                            break;
                        }
                    }
                    else if (search_fields[j].type == TS_STR){
                        if (search_fields[j].is_actual == tupleA.fields[j].is_actual &&
                        my_strcmp(search_fields[j].data.string_field, tupleA.fields[j].data.string_field) == 0){
                            fields_match = 0;  // Fields do not match
                            break;
                        }
                    }
                }
                    
            }

            if (fields_match) {
                return 1;  // Found the tuple, return success
            }
        }
        
        return 1; // Found the tuple, return success
    }
    return 0;
}

void copyFieldArrayToTuple(tuple_struct *destTuple, field_t *fieldArray, int numFields) {
    destTuple->number_of_fields = numFields;

    for (int i = 0; i < numFields; ++i) {
        memcpy(&(destTuple->fields[i]), &(fieldArray[i]), sizeof(field_t));
    }
}

int searchMatchingTupleAsk(tuple_ask tuple_asks[], int *tuples_asks_count, char* search_name, int search_num_fields, field_t* search_fields, int max_index){
    for (int i = 0; i < max_index & i < *tuples_asks_count; ++i) {
        int tuple_check = compareTuples(tuple_asks[i].tuple, search_name, search_num_fields, search_fields);
        if (tuple_check){
            return i;
        }
    }
    return -1; // Tuple with the specified name not found
}

int addTupleToSpace(int *tuples_count, char *tuple_name, int num_fields, field_t *tuple_fields, int max_tuples) {
    if (*tuples_count < max_tuples) {
        strcpy(tuples[*tuples_count].tuple_name, tuple_name);
        tuples[*tuples_count].number_of_fields = num_fields;
        copyFieldArrayToTuple(&tuples[*tuples_count], tuple_fields, num_fields);
        (*tuples_count)++;
        return 1; // Success
    } else {
        // Handle the case when the tuple space is full
        return 0; // Failure
    }
}

int removeTupleAskByID(tuple_ask tuple_asks[], int index, int tuple_asks_count, tuple_ask empty){
    //printf("\tremoving tuple nr %d\n", index);
    tuple_asks[index] = tuple_asks[tuple_asks_count - 1];
    tuple_asks[tuple_asks_count - 1] = empty;

    return tuple_asks_count - 1;
}

int removeTupleByID(tuple_struct tuples[], int index, int tuples_count, tuple_struct empty){
    //printf("\tremoving tuple nr %d\n", index);
    tuples[index] = tuples[tuples_count - 1];
    tuples[tuples_count - 1] = empty;

    return tuples_count - 1;
}

int searchMatchingTuple(tuple_struct tuples[], int *tuples_count, char* search_name, int search_num_fields, field_t* search_fields, int max_index){
    for (int i = 0; i < max_index & i < *tuples_count; ++i) {
        int tuple_check = compareTuples(tuples[i], search_name, search_num_fields, search_fields);
        if (tuple_check){
            return i;
        }
    }
    return -1; // Tuple with the specified name not found
}

void printTupleArray(tuple_struct tuples[], int size, int stats[]) {
    printf("\033[2J\033[H");
    printf("+--------------------+--------+------------------+------------------+\n");
    printf("|     Tuple Name     | Fields |     Field 1      |     Field 2      |\n");
    printf("+--------------------+--------+------------------+------------------+\n");
    for (int i = 0; i < size; ++i) {
        if (strlen(tuples[i].tuple_name) > 0){
            printf("| %*s | %*d |", 18, tuples[i].tuple_name, 6, tuples[i].number_of_fields);
        }
        else{
            printf("| %*c | %*d |", 18, '-', 6, tuples[i].number_of_fields);
        }
        
        
        for (int j = 0; j < 2; ++j) {
            if (j < tuples[i].number_of_fields) {
                if (tuples[i].fields[j].is_actual) {
                    if (tuples[i].fields[j].type == 0) {
                        printf(" Int: %11d ", tuples[i].fields[j].data.int_field);
                    } else if (tuples[i].fields[j].type == 1) {
                        printf(" Float: %9.2f ", tuples[i].fields[j].data.float_field);
                    } else if (tuples[i].fields[j].type == 2) {
                        printf(" Str: %*.*s ", 11, 11, tuples[i].fields[j].data.string_field);
                    }
                }
                else {
                    if (tuples[i].fields[j].type == 0) {
                        printf(" Int: %11c ", '?');
                    } else if (tuples[i].fields[j].type == 1) {
                        printf(" Float: %11c ", '?');
                    } else if (tuples[i].fields[j].type == 2) {
                        printf(" String: %11c ", '?');
                    }
                }
                printf("|");
            } else {
                printf(" %*s |", 16, "-"); // Placeholder for empty field
            }
        }
        printf("\n");
    }
    printf("+--------------------+--------+------------------+------------------+\n");
    printf("+----------------------------------------------------------+\n");
    printf("|                        Statistics                        |\n");
    printf("+----------------+-------+----------------------+----------+\n");
    for (int i = 0; i < 4; ++i) {
        if (i<3){
            printf("| %-*s | %*d ",14, stats_labels[i], 5, stats[i]);
            printf("| %-*s | %*d |\n",20, stats_labels[i+4], 8, stats[i+4]);
        }
        else{
            float avgSize = stats[7]/stats[6];
            printf("| %-*s | %*d ",14, stats_labels[i], 5, stats[i]);
            printf("| %-*s | %*.*f |\n",20, stats_labels[i+4], 8, 2, avgSize);
        }
    }
    printf("+----------------+-------+----------------------+----------+\n");
}
