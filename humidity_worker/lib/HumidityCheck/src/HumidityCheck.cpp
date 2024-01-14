#include "HumidityCheck.h"

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

int humidity(float n)
{
    if (n > 70.0)
        return 1;

    return 0;
}