#ifndef LIB_H
#define LIB_H

#include <stdint.h>


//Representa el punto de entrada de un proceso, similar a la función main en un programa C.
typedef int (*ProcessEntry)(uint64_t argc, char **argv);


void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int myStrlen(const char * s);
void myStrncpy(char *destination, const char *source, unsigned int n);
void myStrcpy(char *dest, const char *src);
int argCount(char** arr);
int myStrcmp(const char *s1, const char *s2);
char *cpuVendor(char *result);

void acquire(uint8_t* lock);
void release(uint8_t* lock);

#endif