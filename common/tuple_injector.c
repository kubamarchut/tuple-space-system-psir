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
   
    /* make a template */
    my_template[0].is_actual = TS_YES;
    my_template[0].type = TS_FLOAT;          /* need to specify the type */
    my_template[0].data.float_field = -457.8085;          /* need to specify the type */
    /* retrieveand remove a tuple with temperature */
    /* some other process must have produced a tuple matching the template */
    int cmd_result = ts_out("temperature", my_template, 1);      /* ("temperature",?float) */
    if (cmd_result == TS_FAILURE){
        return -1;
    }
    sleep(10);
    
    /* transform a previously usedusedtuple into a template */
    my_tuple[0].is_actual = TS_YES;
    my_tuple[1].is_actual = TS_YES;
    my_tuple[0].type = TS_INT;
    my_tuple[1].type = TS_FLOAT;
    my_tuple[0].data.int_field = -128;
    my_tuple[1].data.float_field = -3.14;

    cmd_result = ts_out("nice_constants", my_tuple, 2);      /* ("nice_constants",?int,?float) */
    if (cmd_result == TS_FAILURE){
        return -1;
    }
        
    return 0;
}