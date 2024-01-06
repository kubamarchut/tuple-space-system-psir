#include "utilities.h"
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

void printTimestamp() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    struct tm *local_time = localtime(&current_time.tv_sec);
    char time_string[20];
    strftime(time_string, sizeof(time_string), "%x %H:%M:%S", local_time);

    printf("%s.%03ld", time_string, (long int)current_time.tv_usec / 1000);
}

void printError(int line, char *filename, char* message) {
    printTimestamp();
    printf(" - ERROR occured (%s:%d) %s\n", filename, line, message);
}

int intToBytes(int num, int index){
    int pos = (sizeof(int) - 1) * 8;
    pos -= index * 8;

    return (num >> pos) & 0xFF;
}

int bytesToInt(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4) {
    int result = 0;

    result |= byte4;
    result <<= 8;
    result |= byte3;
    result <<= 8;
    result |= byte2;
    result <<= 8;
    result |= byte1;

    return result;
}

int floatToBytes(float number, int index){
    unsigned int* ptr = (unsigned int*)&number;
    unsigned int num = *ptr;

    int pos = (sizeof(int) - 1) * 8;
    pos -= index * 8;

    return (num >> pos) & 0xff;
}

float bytesToFloat(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4) {
    unsigned int combinedBytes = 0;

    combinedBytes |= byte4;
    combinedBytes <<= 8;
    combinedBytes |= byte3;
    combinedBytes <<= 8;
    combinedBytes |= byte2;
    combinedBytes <<= 8;
    combinedBytes |= byte1;

    float result;
    memcpy(&result, &combinedBytes, sizeof(float));

    return result;
}