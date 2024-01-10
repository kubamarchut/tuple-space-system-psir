#include <stdio.h>
#include <stdlib.h>
#include "src/HumidityCheck.h"

void test(int testSubject, int expectedValue)
{
    int givenValue = humidity(testSubject);

    printf("\"Is prime\" test for %d ", testSubject);
    if (givenValue == expectedValue)
    {
        printf("successful returned value %d\n", givenValue);
    }
    else
    {
        printf("unsuccessful returned value %d expected %d\n", givenValue, expectedValue);
        exit(-1);
    }
}

int main()
{
    printf("Testing prime function\n");

    /* Should return true */
    test(1, 1);
    test(2, 1);
    test(3, 1);
    test(13, 1);
    test(14, 1);

    /* Should return false */
    test(40, 0);
    test(100, 0);
    test(160, 0);
    test(20, 0);
    test(2138, 0);

    return 0;
}