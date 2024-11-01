#ifndef LIBRARY_USER_H
#define LIBRARY_USER_H

#include <stdint.h>

void clearIfNotEnoughSpace(int linesItNeeds);

int strcmp(char *s1, char *s2);

static int strConcat(char *str1, char *str2);

void strcpy(char *destination, const char *source);

char *strncpy(char *dest, const char *src, int count);

/* int strlen(const char * s) {
    int i = 0;
    while(s[i] != '\0') {
        if(s[i] == '\t') {
            i+=4;
        }
        i++;
    }
    return i;
} */

//----------------------------------------------------------------------------------------------------
void putChar(char c, uint32_t hexColor);

char getChar();

void swap(char *x, char *y);

char toLower(char c);
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
int putString(char * c, uint32_t color);
//----------------------------------------------------------------------------------------
static int intToString(int num, char *str);

static int strConcat(char *str1, char *str2);

static int stringToInt(char * num);

int printf(const char * format, ...);

int scanf(const char *fmt, ...);

static int readFromKeyboard(char * buffer);

#endif