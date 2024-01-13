#include "HumidityCheck.h"

int humidity(int n)
{
    if (n > 700)
        return 0;

    return 1;
}