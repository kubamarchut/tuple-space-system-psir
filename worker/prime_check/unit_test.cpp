#include <stdio.h>
#include "prime_check.h"

void test(int testSubject, int expectedValue){
    int givenValue = isPrime(testSubject);

    printf("\"Is prime\" test for %d ", testSubject);
    if (givenValue == expectedValue){
        printf("successful returned value %d\n", givenValue);
    }
    else{
        printf("unsuccessful returned value %d expected %d\n", givenValue, expectedValue);
    }
}

int main(){
    printf("Testing prime function\n");

    /* Should return true */
    test(1, 1);
    test(2, 1);
    test(3, 1);
    test(13, 1);
    test(2137, 1);

    /* Should return false */
    test(4, 0);
    test(10, 0);
    test(16, 0);
    test(100, 0);
    test(2138, 0);

    return 0;
}