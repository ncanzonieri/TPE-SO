#ifndef LIBRARY_USER_H
#define LIBRARY_USER_H

#include <stdint.h>

#define EOF -1

typedef int (*ProcessEntry)(uint64_t argc, char **argv);

void clearIfNotEnoughSpace(int linesItNeeds);

int strcmp(char *s1, char *s2);

int strConcat(char *str1, char *str2);

int strcpy(char *destination, const char *source);

char *strncpy(char *dest, const char *src, int count);

int strlen(const char * s);

void putChar(char c, uint32_t hexColor);

char getChar();

void swap(char *x, char *y);

char toLower(char c);

int putString(char * c, uint32_t color);

char *strchr(char * input, int character);

int intToString(int num, char *str);

int hexToString(uint64_t num, char *toPrint);

int strConcat(char *str1, char *str2);

int stringToInt(char * num);

int printf(const char * format, ...);

int scanf(const char *fmt, ...);

int readFromKeyboard(char * buffer);

int ownRand(int from, int to);

int atoi(const char * input);
char* itoa(int num, char* str);


#endif