#ifndef LIBRARY_USER_H
#define LIBRARY_USER_H

#include <stdint.h>

void clearIfNotEnoughSpace(int linesItNeeds);

int strcmp(char *s1, char *s2);

int strConcat(char *str1, char *str2);

void strcpy(char *destination, const char *source);

char *strncpy(char *dest, const char *src, int count);

int strlen(const char * s);

void putChar(char c, uint32_t hexColor);

char getChar();

void swap(char *x, char *y);

char toLower(char c);

void* myMalloc(uint64_t size); // malloc

void myFree(void* ptr); // free

int putString(char * c, uint32_t color);

int intToString(int num, char *str);

int strConcat(char *str1, char *str2);

int stringToInt(char * num);

int printf(const char * format, ...);

int scanf(const char *fmt, ...);

int readFromKeyboard(char * buffer);

int ownRand(int from, int to);

#endif