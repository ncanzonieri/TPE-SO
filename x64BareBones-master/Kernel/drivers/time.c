// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <naiveConsole.h>
#include <scheduler.h>
#define DIM 400

static unsigned long ticks = 0;

uint64_t timer_handler(uint64_t rsp) {
	ticks++;
    rsp = (uint64_t) scheduler((void*) rsp);
    return rsp;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

uint64_t ms_elapsed(){
    return ticks * 55;
}

uint64_t sleep(uint64_t ms){
    uint64_t ini = ms_elapsed();
    uint64_t now = ini;
    _sti();
    while(now - ini <= ms){
        now=ms_elapsed();
    }
    _cli();
    return 1;
}

static uint8_t reformatToDec(uint8_t value){
	// esta funcion local solo tiene sentido en el contexto
	// de que los formatos en rtc escriben un numero decimal en 1B donde se lee hexadecimal
	// por eso value son dos digitos y no tiene 'letras'
	return value%16 + (value/16)*10;
}

uint8_t rtc(uint8_t timeOption){
    return reformatToDec(rtcDriver(timeOption));
}