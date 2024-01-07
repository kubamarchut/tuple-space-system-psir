#ifndef TUPLE_SPACE_H
#define TUPLE_SPACE_H

#define TS_YES      1
#define TS_NO       0
#define TS_INT      0
#define TS_FLOAT    1
#define TS_SUCCESS  1
#define TS_FAILURE  0

#define TS_CMD_OUT        0
#define TS_CMD_IN         1
#define TS_CMD_RD         2
#define TS_CMD_NO_TUPLE   3

typedef struct {
    int is_actual;  /* does the data member contains data */
    int type;       /* what is the type of the data member */
    union {
        int int_field;
        float float_field;
        char* string_field;
    } data;
} field_t;          /* a new type corresponding to one field of a tuple*/

/* API */
/* these functions return TS_SUCCESS or TS_FAILURE */
/* parameters : tuple name , other fields , no. of other fields */

// this function should adds a tuple with specified fields to the tuple space
int ts_out (char*, field_t *, int);

// this function should retrieve a tuple matching the specified fields from the tuple space and delete it
int ts_inp (char*, field_t *, int);

// this function should retrieve a tuple matching the specified fields from the tuple space and without deleting it
int ts_rdp (char*, field_t *, int);
#endif