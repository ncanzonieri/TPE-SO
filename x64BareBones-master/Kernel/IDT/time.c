#include <time.h>

static unsigned long ticks = 0;
extern uint8_t rtcDriver(uint8_t opt);

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

static uint8_t reformatToDec(uint8_t value){
	// esta funcion local solo tiene sentido en el contexto
	// de que los formatos en rtc escriben un numero decimal en 1B donde se lee hexadecimal
	// por eso value son dos digitos y no tiene 'letras'
	return value%10 + (value/10)*16;
}

struct tm * now(){
	struct tm * ans;
	ans->day=reformatToDec(rtcDriver(7));
	ans->month=reformatToDec(rtcDriver(8));
	ans->year=reformatToDec(rtcDriver(9))+2000; // en el byte que recibe solo están los últimos dos dígitos
	ans->sec=reformatToDec(rtcDriver(0));
	ans->min=reformatToDec(rtcDriver(2));
	ans->hour=reformatToDec(rtcDriver(4))-3; // pasando de UTC a hora de BS As
}

char * getDate(){
    int day, month, year;

    struct tm *ans = now();

    day = ans->day;
    month = ans->month;
    year = ans->year;

    static char date[20];
    sprintf(date, "%02d/%02d/%04d", day, month, year);
    return date;
}

char * getTime(){
	int sec, min, hour;

    struct tm *ans = now();

    sec = ans->sec;
    min = ans->min;
    hour = ans->hour;

    static char time[20];
    sprintf(time, "%02d:%02d:%02d", hour, min, sec);
    return time;
}