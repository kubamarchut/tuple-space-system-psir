#ifndef HUMIDITY_CHECK_H
#define HUMIDITY_CHECK_H
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max);
int humidity(float n, bool alarn_on);

#endif /* HUMIDITY_CHECK_H */