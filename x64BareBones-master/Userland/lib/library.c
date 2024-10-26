#include <library.h>


// chequear estas funciones


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
    return putString(str);
}

#define BUFFER_DIM 20
