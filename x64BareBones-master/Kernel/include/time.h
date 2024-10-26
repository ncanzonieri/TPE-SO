#ifndef MY_TIME_H_
#define MY_TIME_H_

#include <stdint.h>

typedef struct tm {
    uint8_t sec, min, hour, day, month;
    uint16_t year;
} timeStruct;
timeStruct * now();
void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
uint64_t ms_elapsed();
char * getDate();
char * getTime();
uint64_t sleep(uint64_t ms);

#endif
