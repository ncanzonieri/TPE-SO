#include <stdint.h>
#include<stdargs.h>
#define DIM 100
#define STDOUT 1
#define BUFFER_DIM 30
int strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] == s2[i])
    {
        if (s1[i] == '\0' && s2[i] == '\0')
            return 0; 
        i++;
    }
    return s1[i] - s2[i]; 
}

void strcpy(char *destination, const char *source)
{
    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
    return;
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

//----------------------------------------------------------------------------------------------------
char getChar(){
    uint8_t c;
    int size = 0;
    while(size!=1){
        size = sys_read(&c, 1, &size);
    }
    return c;
}

void swap(char *x, char *y)
{
    char t = *x;
    *x = *y;
    *y = t;
}

char toLower(char c) {
    if(c >= 'A' && c <= 'Z') {
        return c - 'A' + 'a';
    }
    return c;
}
//----------------------------------------------------------------------------------------
/*void putcharColor(char c, uint32_t color) {
    _sys_write(STDOUT, &c, 1, color);
}

void printStringColor(const char * str, uint32_t color) {
    for(int i=0; str[i] != 0; i++) {
        putcharColor(str[i], color);
    }
}

void putchar(char c) {
    putcharColor(c, WHITE);
}*/
int putString(char * c, uint32_t color) {
    uint32_t length = strlen(c);
    sys_write(STDOUT,(uint8_t *)c,length,color);
    return length;
}
//----------------------------------------------------------------------------------------
static int readFromKeyboard(char * buffer) {
    int i=0;
    char c;
    while (c != ' ' && c != '\t' && c != '\n' && i < BUFFER_DIM) {
        c = getChar();
        buffer[i++] = c;
    }
    return i;
}
//---------------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------------
void scanf(const char * format, ...) {
    va_list variables;

    va_start(variables, format);

    char str[DIM];
    char buffer[BUFFER_DIM];
    int index = 0, fmtPos = 0;

     while(format[fmtPos] != '\0'){
        if(format[fmtPos] == '%'){
            printf(str);
            fmtPos++;
            switch(format[fmtPos]){
                case 'd': //int
                    readFromKeyboard(buffer);
                    int * number;
                    number = va_arg(variables, int*);
                    *number = stringToInt(buffer);
                    break;
                case 's': //string
                    readFromKeyboard(buffer);
                    char ** string;
                    string = va_arg(variables, char**);
                    *string = buffer;
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
}