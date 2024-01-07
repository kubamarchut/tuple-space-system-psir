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
