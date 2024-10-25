#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

struct tm {
    uint8_t sec, min, hour, day, month;
    uint16_t year;
};
struct tm * now();
void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
char * getDate();
char * getTime()

#endif
