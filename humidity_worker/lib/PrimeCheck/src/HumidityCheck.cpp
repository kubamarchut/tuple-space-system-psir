#include "HumidityCheck.h"

int humidity(int n)
{
    for (int i = 2; i <= n / 2; i++)
    {
        if (n / 100 >= 70)
            return 0;
    }
    return 1;
}