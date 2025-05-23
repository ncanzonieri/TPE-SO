#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int strlen(const char * s);
void strncpy(char *destination, const char *source, unsigned int n);
int argCount(char** arr);
char *cpuVendor(char *result);

#endif