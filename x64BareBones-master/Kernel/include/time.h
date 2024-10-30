#ifndef MY_TIME_H_
#define MY_TIME_H_

#include <stdint.h>

typedef struct tm {
    uint8_t sec, min, hour, day, month;
    uint16_t year;
} timeStruct;

extern uint8_t rtcDriver(uint8_t opt);
extern void _cli();
extern void _sti();

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
uint64_t ms_elapsed();
uint64_t sleep(uint64_t ms);
uint8_t rtc(uint8_t timeOption);

#endif
