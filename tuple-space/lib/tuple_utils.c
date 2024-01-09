#include "tuple_utils.h"
#include "../../common/lib/tuple_space.h"

#include <stdio.h>
#include <string.h>

const char *stats_labels[4] = {
        "No. messages \"out\" type",
        "No. messages \"in\" type",
        "No. messages \"rd\" type",
        "No. no matching tuple",
    };

int removeTupleByID(tuple_struct tuples[], int index, int tuples_count, tuple_struct empty){
    //printf("\tremoving tuple nr %d\n", index);
    tuples[index] = tuples[tuples_count - 1];
    tuples[tuples_count - 1] = empty;

    return tuples_count - 1;
}

int searchTupleByName(tuple_struct tuples[], char* search_name, int max_index) {
    for (int i = 0; i < max_index; ++i) {
        if (strcmp(tuples[i].tuple_name, search_name) == 0) {
            return i; // Found the tuple, return its index
        }
    }
    return -1; // Tuple with the specified name not found
}

void printTupleArray(tuple_struct tuples[], int size, int stats[]) {
    printf("\033[2J\033[H");
    printf("+--------------------+---------------+--------------------+--------------------+\n");
    printf("|     Tuple Name     | No. of Fields |      Field 1       |      Field 2       |\n");
    printf("+--------------------+---------------+--------------------+--------------------+\n");
    for (int i = 0; i < size; ++i) {
        if (strlen(tuples[i].tuple_name) > 0){
            printf("| %*s | %*d |", 18, tuples[i].tuple_name, 13, tuples[i].number_of_fields);
        }
        else{
            printf("| %*c | %*d |", 18, '-', 13, tuples[i].number_of_fields);
        }
        
        
        for (int j = 0; j < 2; ++j) {
            if (j < tuples[i].number_of_fields) {
                if (tuples[i].fields[j].is_actual) {
                    if (tuples[i].fields[j].type == 0) {
                        printf(" Int: %13d ", tuples[i].fields[j].data.int_field);
                    } else if (tuples[i].fields[j].type == 1) {
                        printf(" Float: %11.2f ", tuples[i].fields[j].data.float_field);
                    } else if (tuples[i].fields[j].type == 2) {
                        printf(" String: %s ", tuples[i].fields[j].data.string_field);
                    }
                }
                else {
                    if (tuples[i].fields[j].type == 0) {
                        printf(" Int: %13c ", '?');
                    } else if (tuples[i].fields[j].type == 1) {
                        printf(" Float: %11c ", '?');
                    } else if (tuples[i].fields[j].type == 2) {
                        printf(" String: %c ", '?');
                    }
                }
                printf("|");
            } else {
                printf(" %*s |", 18, "-"); // Placeholder for empty field
            }
        }
        printf("\n");
    }
    printf("+--------------------+---------------+--------------------+--------------------+\n");
    printf("+------------------------------------+\n");
    printf("|            Statistics              |\n");
    printf("+----------------------------+-------+\n");
    for (int i = 0; i < 4; ++i) {
        printf("| %-*s | %*d |\n",26, stats_labels[i], 5, stats[i]);
    }
    printf("+----------------------------+-------+\n");
}
