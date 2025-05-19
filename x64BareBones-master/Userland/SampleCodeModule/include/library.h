#ifndef LIBRARY_USER_H
#define LIBRARY_USER_H

#include <stdint.h>

void clearIfNotEnoughSpace(int linesItNeeds);

int strcmp(char *s1, char *s2);

static int strConcat(char *str1, char *str2);

void strcpy(char *destination, const char *source);

char *strncpy(char *dest, const char *src, int count);

int strlen(const char * s);

void putChar(char c, uint32_t hexColor);

char getChar();

void swap(char *x, char *y);

char toLower(char c);


int putString(char * c, uint32_t color);

static int intToString(int num, char *str);

static int strConcat(char *str1, char *str2);

static int stringToInt(char * num);

int printf(const char * format, ...);

int scanf(const char *fmt, ...);

static int readFromKeyboard(char * buffer);

int ownRand(int from, int to);

#endif