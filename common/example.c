#include "lib/tuple_space.h"
#include "lib/udp_handler.h"
#include <stdio.h>
#include <unistd.h>

#define IP_ADDR "127.0.0.1"
#define PORT "5000"

int main(void)
{
    //UDP init
    int s = init_udp_socket(IP_ADDR, PORT);

    int nice_power;
    double temp, pi;
    field_t my_tuple[2];    /* an array of fields (name not included) */
    field_t my_template[1];

    printf("starting \"out\" operation\n");
    /* make a tuple */
    my_tuple[0].is_actual = TS_YES;
    my_tuple[0].type = TS_INT;
    my_tuple[0].data.int_field = 128;
    my_tuple[1].is_actual = TS_YES;
    my_tuple[1].type = TS_FLOAT;
    //my_tuple[1].data.string_field = "tete tete tete";
    my_tuple[1].data.float_field = 3.14;

    /* add a tuple to the tuple space */
    int out_result = ts_out("nice_constants", my_tuple, 2);      /* ("nice_constants",128,3.14) */
    if (out_result == TS_FAILURE){
        printf("an error encourted\n");
        return -1;
    }
    else if (out_result == TS_SUCCESS){
        printf("sent tuple\n");
    }
    sleep(1);
    printf("\n");

    /* make a template */
    my_template[0].is_actual = TS_NO;
    my_template[0].type = TS_FLOAT;          /* need to specify the type */
    /* retrieveand remove a tuple with temperature */
    /* some other process must have produced a tuple matching the template */
    int cmd_result = ts_inp("temperature", my_template, 1);      /* ("temperature",?float) */
    if (cmd_result == TS_FAILURE){
        printf("an error encourted\n");
        return -1;
    }
    else if (cmd_result == TS_NO_TUPLE){
        printf("no tuple matched your tuple template\n");
    }
    else if (cmd_result == TS_SUCCESS){
        printf("received correct tuple\n");
        temp = my_template[0].data.float_field;
    }
    sleep(1);
    printf("\n");

    /* transform a previously usedusedtuple into a template */
    my_tuple[0].is_actual = TS_NO;
    my_tuple[1].is_actual = TS_NO;

    printf("starting \"rdp\" operation\n");
    cmd_result = ts_rdp("nice_constants", my_tuple, 2);      /* ("nice_constants",?int,?float) */
    if (cmd_result == TS_FAILURE){
        printf("an error encourted");
        return -1;
    }
    else if (cmd_result == TS_NO_TUPLE){
        printf("no tuple matched your tuple template\n");
    }
    else if (cmd_result == TS_SUCCESS){
        printf("received correct tuple\n");
        nice_power = my_tuple[0].data.int_field;    /* 128 from the tuple spacespace*/
        pi = my_tuple[1].data.float_field;          /* 3.14 from the tuple spacespace*/

        printf("gotten \n\tnice_power: %6d", nice_power);
        printf("\n\tpi: %14f\n\n", pi);
    }
    sleep(1);

    my_tuple[0].is_actual = TS_NO;
    my_tuple[1].is_actual = TS_NO;

    printf("starting \"inp\" operation\n");
    cmd_result = ts_inp("nice_constants", my_tuple, 2);      /* ("nice_constants",?int,?float) */
    if (cmd_result == TS_FAILURE){
        printf("an error encourted\n");
        return -1;
    }
    else if (cmd_result == TS_NO_TUPLE){
        printf("no tuple matched your tuple template\n");
    }
    else if (cmd_result == TS_SUCCESS){
        printf("received correct tuple\n");
        nice_power = my_tuple[0].data.int_field;    /* 128 from the tuple spacespace*/
        pi = my_tuple[1].data.float_field;          /* 3.14 from the tuple spacespace*/

        printf("gotten \n\tnice_power: %6d", nice_power);
        printf("\n\tpi: %14f\n\n", pi);
    }
    
    return 0;
}