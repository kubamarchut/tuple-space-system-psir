#ifndef UTILITIES_H
#define UTILITIES_H

void printTimestamp();

void printError(int line, char *filename, char* message);

int intToBytes(int number, int index);
int floatToBytes(float number, int index);

int bytesToInt(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);
float bytesToFloat(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);

#endif /* UTILITIES_H */