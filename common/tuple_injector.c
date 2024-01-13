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
   
    printf("starting \"out\" operation\n");
    my_template[0].is_actual = TS_YES;
    my_template[0].type = TS_FLOAT;
    my_template[0].data.float_field = -457.8085;
    
    
    int cmd_result = ts_out("temperature", my_template, 1);
    if (cmd_result == TS_FAILURE){
        return -1;
    }
    sleep(5);
    
    printf("starting \"out\" operation\n");
    
    my_tuple[0].is_actual = TS_YES;
    my_tuple[1].is_actual = TS_YES;
    my_tuple[0].type = TS_INT;
    my_tuple[1].type = TS_FLOAT;
    my_tuple[0].data.int_field = -128;
    my_tuple[1].data.float_field = -3.14;

    cmd_result = ts_out("nice_constants", my_tuple, 2);
    if (cmd_result == TS_FAILURE){
        return -1;
    }
        
    return 0;
}