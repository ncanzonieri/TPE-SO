#ifndef LIB_H
#define LIB_H

#include <stdint.h>

//Representa el punto de entrada de un proceso, similar a la función main en un programa C.
typedef int (*ProcessEntry)(uint64_t argc, char **argv);


void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int myStrlen(const char * s);
void myStrncpy(char *destination, const char *source, unsigned int n);
int argCount(char** arr);
char *cpuVendor(char *result);

#endif