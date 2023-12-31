#ifndef TUPLE_SPACE_H
#define TUPLE_SPACE_ H
#define TS_YES 1
#define TS_NO 0
#define TS_INT 0
#define TS_FLOAT 1
#define TS_SUCCESS 1
#define TS_FAILURE 0

typedef
struct
10.
int
is_actual ;;/* does the data member contains data */
11.
int
type ;;/* what is the type of the data member
12.
union
13.
int
int_field
14.
float
float_field
15.
}
data;
16.
}
field_t ;;/* a new type corresponding to one field of a tuple
17.
/* API */
18.
/*
these functions return TS_SUCCESS or TS_FAILURE */
19.
/*
parameters : tuple name , other fields , no. of other fields*/
20.
int
ts_out (char*, field_t *, int
21.
int
ts_inp (char*, field_t *, int
22.
int
ts_rdp (char*, field_t *, int
23.
#endif