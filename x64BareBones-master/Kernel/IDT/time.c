#include <time.h>
#include <stdint.h>

static unsigned long ticks = 0;
extern uint8_t rtcDriver(uint8_t opt);
typedef struct tm {
    uint8_t sec, min, hour, day, month;
    uint16_t year;
} timeStruct;
static int dayTab[13]= { 29, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int bisiesto(int anio) {
    return (anio%4 == 0 && anio%100!= 0) || anio%400 == 0;
}

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

timeStruct * now(){
	timeStruct * ans;
    uint8_t utcHour = reformatToDec(rtcDriver(4));
    uint8_t utcDay = reformatToDec(rtcDriver(7));
    uint8_t utcMonth = reformatToDec(rtcDriver(8));
    uint8_t utcYear = reformatToDec(rtcDriver(9))+2000;
    if(utcHour<3){
        utcHour+=21;
        if(utcDay==1){
            if(utcMonth==1){
                utcMonth=12;
                utcYear--; //no analizo el caso de 2000, no tiene sentido
            }else{
                utcMonth--;
            }
            if(bisiesto(utcYear) && utcMonth==2){
                utcDay=dayTab[0];
            }else{
                utcDay=dayTab[utcMonth];
            }
        }else{
            utcDay--;
        }
    }else{
        utcHour-=3;
    }
	ans->sec=reformatToDec(rtcDriver(0));
	ans->min=reformatToDec(rtcDriver(2));
	ans->hour=utcHour;
    ans->day=utcDay;
    ans->month=utcMonth;
    ans->year=utcYear;
}

char * getDate(){
    int day, month, year;

    timeStruct *ans = now();

    day = ans->day;
    month = ans->month;
    year = ans->year;

    static char date[20];
    sprintf(date, "%02d/%02d/%04d", day, month, year);
    return date;
}

char * getTime(){
	int sec, min, hour;

    timeStruct *ans = now();

    sec = ans->sec;
    min = ans->min;
    hour = ans->hour;

    static char time[20];
    sprintf(time, "%02d:%02d:%02d", hour, min, sec);
    return time;
}