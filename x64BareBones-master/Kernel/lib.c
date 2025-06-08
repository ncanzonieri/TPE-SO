#include <stdint.h>
#define NULL ((void*)0)

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

int myStrlen(const char * s) {
    int i = 0;
    while(s[i] != '\0') {
        if(s[i] == '\t') {
            i+=4;
        }
        i++;
    }
    return i;
} 

void myStrncpy(char *destination, const char *source, unsigned int n){
    unsigned int i = 0;
    while (i < n && source[i] != '\0'){
        destination[i] = source[i];
        i++;
    }
    while (i < n){
        destination[i] = '\0';
        i++;
    }
    return;
}

void myStrcpy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
}

int argCount(char** arr) {
	int i = 0;
	while (arr && arr[i] != NULL)
		i++;
	return i;
}

int myStrcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (int)(*(const unsigned char *)s1 - *(const unsigned char *)s2);
}

char* myItoa(int num, char* str) {
    uint64_t i = 0;
    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }
    if (num == 0) {
        str[i++] = '0';
    } else {
        while (num != 0) {
            str[i++] = (num % 10) + '0';
            num /= 10;
        }
    }
    if (isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    uint64_t start = 0;
    uint64_t end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
    return str;
}

uint64_t myStrcat(char* dest, const char* src) {
    uint64_t destLen = 0;
    while (dest[destLen] != '\0') {
        destLen++;
    }
    uint64_t i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
    return destLen + i;
}