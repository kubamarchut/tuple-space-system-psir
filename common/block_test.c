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
    field_t my_tuple[2];    
    field_t my_template[1];
   
    printf("starting \"in\" operation\n");

    my_template[0].is_actual = TS_NO;
    my_template[0].type = TS_FLOAT;
    
    int cmd_result = ts_in("temperature", my_template, 1);
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
        printf("gotten \n\ttemp: %12f\n\n", temp);
    }
    sleep(1);
    printf("\n");
    
    my_tuple[0].type = TS_INT;
    my_tuple[1].type = TS_FLOAT;
    my_tuple[0].is_actual = TS_NO;
    my_tuple[1].is_actual = TS_NO;

    printf("starting \"rd\" operation\n");
    cmd_result = ts_rd("nice_constants", my_tuple, 2);
    if (cmd_result == TS_FAILURE){
        printf("an error encourted\n");
        return -1;
    }
    else if (cmd_result == TS_NO_TUPLE){
        printf("no tuple matched your tuple template\n");
    }
    else if (cmd_result == TS_SUCCESS){
        printf("received correct tuple\n");
        nice_power = my_tuple[0].data.int_field;
        pi = my_tuple[1].data.float_field;

        printf("gotten \n\tnice_power: %6d", nice_power);
        printf("\n\tpi: %14f\n\n", pi);
    }
        
    return 0;
}