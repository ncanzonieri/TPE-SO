#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <naiveConsole.h>
#define DIM 400
#define STDOUT 1
#define WHITE 0xFFFFFF

/*
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

void now(timeStruct * ans);
void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
uint64_t ms_elapsed();
char * getDate(timeStruct* ans);
char * getTime(timeStruct* ans);
uint64_t sleep(uint64_t ms);

#endif
*/


static unsigned long ticks = 0;

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

void now(timeStruct * ans){
    uint8_t utcHour = reformatToDec(rtcDriver(4));
    uint8_t utcDay = reformatToDec(rtcDriver(7));
    uint8_t utcMonth = reformatToDec(rtcDriver(8));
    uint8_t utcYear = reformatToDec(rtcDriver(9));
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

char * getDate(char* date){
    timeStruct ans;
    now(&ans);
    int day, month, year;

    day = ans.day;
    month = ans.month;
    year = ans.year;

    date[2]='/';
    date[5]='/';
    date[6]='2';
    date[7]='0';
    date[10]=0;
    for(int i=0; i<2; i++){
        date[1-i]=day%10+'0';
        day/=10;
        date[4-i]=month%10+'0';
        month/=10;
        date[9-i]=year%10+'0';
        year/=10;
    }
    return date;
}

char * getTime(char* time){
    timeStruct ans;
    now(&ans);
	int sec, min, hour;

    sec = ans.sec;
    min = ans.min;
    hour = ans.hour;

    time[2]=':';
    time[5]=':';
    for(int i=0; i<2; i++){
        time[1-i]=(hour%10)+'0';
        hour/=10;
        time[4-i]=(min%10)+'0';
        min/=10;
        time[7-i]=(sec%10)+'0';
        sec/=10;
    }
    time[8]=0;

    return time;
}

/*
static int strConcat(char *str1, char *str2){
    int i = strlen(str1);
    int j = 0;
    while(str2[j] != '\0'){
        str1[i] = str2[j];
        i++;
        j++;
    }
    return i;
}
static int intToString(int num, char *str) {
    int i = 0, j=0;
    char isNegative = 0;
    char aux[10]; 
    // if negative flag to add sign
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }
    if (num == 0) {
        str[i] = '0';
        i++;
    } 
    while (num != 0) {
        aux[j] = (num % 10) + '0';
        num = num / 10;
        j++;
    }
    if (isNegative) {
        str[i] = '-';
        i++;
    }
    for (j = j - 1; j >= 0; j--) {
        str[i] = aux[j];
        i++;
    }
    str[i] = '\0';
    return i;
}
static int stringToInt(char * num){
    char isNegative = 0;
    int i = 0;
    int res = 0;

    if(num[0] == '-'){
        isNegative = 1;
        i++;
    }

    while(num[i] != '\0'){
        res = res*10 + num[i] - '0';
        i++;
    }
    
    if(isNegative){
        res = -res;
    }
    
    return res;
}

int printf(const char * format, ...){
    va_list variables;

    va_start(variables, format);

    char str[DIM];
    int index = 0, fmtPos = 0;

    while(format[fmtPos] != '\0'){
        if(format[fmtPos] == '%'){
            fmtPos++;
            switch(format[fmtPos]){
                case 'd': //int
                    index += intToString(va_arg(variables,int),str+index);
                    break;
                case 's': //string
                    index+=strConcat(str,va_arg(variables,char*));
                    break;
                default:
                    break;
            }
            fmtPos++;
        }else{
            str[index] = format[fmtPos++];
            index++;
        }
    }
    str[index] = '\0';
    va_end(variables);
    return putString(str, WHITE);
}

int putString(char * c, uint32_t color) {
    uint32_t length = strlen(c);
    sys_write(STDOUT,(uint8_t *)c,length,color);
    return length;
}

int strlen(const char * s) {
    int i = 0;
    while(s[i] != '\0') {
        if(s[i] == '\t') {
            i+=4;
        }
        i++;
    }
    return i;
}
*/