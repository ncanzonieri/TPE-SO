// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <time.h>
#include <syscalls.h>
#include <library.h>
#define SEC 0
#define MIN 2
#define HOUR 4
#define DAY 7
#define MONTH 8
#define YEAR 9

static int dayTab[13]= { 29, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int bisiesto(int anio) {
    return (anio%4 == 0 && anio%100!= 0) || anio%400 == 0;
}

int actualTime(int argc, char *argv[]){
    uint64_t sec=sys_getTime(SEC);
    uint64_t min=sys_getTime(MIN);
    uint64_t hour=sys_getTime(HOUR);
    hour+= (hour<3)?21:(-3);
    char time[]="00:00:00\n";
    for(int i=0; i<2; i++){
        time[1-i]=hour%10+'0';
        hour/=10;
        time[4-i]=min%10+'0';
        min/=10;
        time[7-i]=sec%10+'0';
        sec/=10;
    }
    clearIfNotEnoughSpace(1);
    sys_write(STDOUT_FD, time,10, 0x00ffffff);
    return 0;
}

int actualDate(int argc, char *argv[]){
    uint64_t year=sys_getTime(YEAR);
    uint64_t month=sys_getTime(MONTH);
    uint64_t day=sys_getTime(DAY);
    uint64_t hour=sys_getTime(HOUR);
    if(hour<3){
        if(day==1){
            if(month==1){
                month=12;
                year--; //no analizo el caso de 2000, no tiene sentido
            }else{
                month--;
            }
            if(bisiesto(year) && month==2){
                day=dayTab[0];
            }else{
                day=dayTab[month];
            }
        }else{
            day--;
        }
    }
    char date[]="00/00/2000\n";
    for(int i=0; i<2; i++){
        date[1-i]=day%10+'0';
        day/=10;
        date[4-i]=month%10+'0';
        month/=10;
        date[9-i]=year%10+'0';
        year/=10;
    }
    clearIfNotEnoughSpace(1);
    sys_write(STDOUT_FD, date,12, 0x00ffffff);
    return 0;
}
