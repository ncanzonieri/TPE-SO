#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <library.h>
#include <syscalls.h>

#define DIM 256
#define STDOUT 1
#define WHITE 0x00FFFFFF
#define BUFFER_DIM 256
#define SCANF_BUFFER_SIZE 256

void clearIfNotEnoughSpace(int linesItNeeds){
    uint32_t y=(uint32_t)(sys_getCoords()>>32);
    uint32_t height=(uint32_t)(sys_getScreenInfo() & 0xffffffff);
    if(y+linesItNeeds*16*sys_getFontInfo()>=height){
        sys_clearScreen();
    }
}

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

int strConcat(char *str1, char *str2){
    int i = strlen(str1);
    int j = 0;
    while(str2[j] != '\0'){
        str1[i] = str2[j];
        i++;
        j++;
    }
    return i;
}

int strcpy(char *destination, const char *source)
{
    int count = 0;
    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
        count++;
    }
    *destination = '\0';
    return count;
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

char getChar(){
    uint8_t c;
    int size = 0;
    while(size!=1){
        size = sys_read(STDIN_FD,&c, 1);
    }
    return c;
}

void putChar(char c, uint32_t hexColor){
    sys_write(STDOUT, &c, 1, hexColor);
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

char * fgets(char *buffer, size_t size) {
    int readBytes = 0;
    char c;
    while(readBytes < (size-1) && (((c = getChar()) != '\n'))) {
        buffer[readBytes++] = c;
    }
    buffer[size-1] = 0;
    return buffer;
}

int readToBlank(char * str, int index) {
    int readBytes = 0;
    for(int i=index; str[i] != 0 && str[i] != '\n' && str[i] != ' ' && str[i] != '\t'; i++) {
        readBytes++;
    }
    return readBytes;
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
    return sys_write(STDOUT,(uint8_t *)c,length,color);
}
//----------------------------------------------------------------------------------------
int intToString(int num, char *str) {
    int i = 0, j=0;
    char isNegative = 0;
    char aux[10]; 
    // if negative flag to add sign
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }
    if (num == 0) {
        str[i] = '0';
        i++;
    } 
    while (num != 0) {
        aux[j] = (num % 10) + '0';
        num = num / 10;
        j++;
    }
    if (isNegative) {
        str[i] = '-';
        i++;
    }
    for (j = j - 1; j >= 0; j--) {
        str[i] = aux[j];
        i++;
    }
    str[i] = '\0';
    return i;
}



int stringToInt(char * num){
    char isNegative = 0;
    int i = 0;
    int res = 0;

    if(num[0] == '-'){
        isNegative = 1;
        i++;
    }

    while(num[i] != '\0'){
        res = res*10 + num[i] - '0';
        i++;
    }
    
    if(isNegative){
        res = -res;
    }
    
    return res;
}

int hexToString(uint64_t num, char *toPrint) {
	char hexa[16] = "0123456789ABCDEF";
	char aux[16];
	int i = 0, j = 0;

	if (num == 0) {
		toPrint[i] = '0';
		i++;
	}

	while (num != 0) {
		aux[j] = hexa[num % 16];
		num = num / 16;
		j++;
	}

	toPrint[i] = '0';
	i++;
	toPrint[i] = 'x';
	i++;

	for (j = j - 1; j >= 0; j--) {
		toPrint[i] = aux[j];
		i++;
	}
	toPrint[i] = '\0';

	return i;
}

// int printf(const char * format, ...){
//     va_list variables;

//     va_start(variables, format);

//     char str[DIM];
//     int index = 0, fmtPos = 0;

//     while(format[fmtPos] != '\0'){
//         if(format[fmtPos] == '%'){
//             fmtPos++;
//             switch(format[fmtPos]){
//                 case 'd': //int
//                     index += intToString(va_arg(variables,int),str+index);
//                     break;
//                 case 's': //string
//                     index+=strConcat(str,va_arg(variables,char*));
//                     break;
//                 default:
//                     break;
//             }
//             fmtPos++;
//         }else{
//             str[index] = format[fmtPos++];
//             index++;
//         }
//     }
//     str[index] = '\0';
//     va_end(variables);
//     return putString(str, WHITE);
// }

int printf(const char *fmt, ...) {
	va_list variables;

	va_start(variables, fmt);
	char toPrint[512];
	unsigned int index = 0;
	while (*fmt != 0) {
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
				case 'd':
					index += intToString(va_arg(variables, int), toPrint + index);
					break;
				case 's':
					index += strcpy(toPrint + index, va_arg(variables, char *));
					break;
				case 'p':
					index += hexToString(va_arg(variables, uint64_t), toPrint + index);
					break;
				case 'c':
					toPrint[index++] = va_arg(variables, int);
					break;
				default:
					break;
			}
			fmt++;
		}
		else
			toPrint[index++] = *fmt++;
	}
	toPrint[index] = 0;
	
	// puts(toPrint);
	va_end(variables);
    return putString(toPrint, WHITE);
}


int scanf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char input[SCANF_BUFFER_SIZE];
    fgets(input, SCANF_BUFFER_SIZE);
    int index = 0;

    // Aux buffer
    char buffer[SCANF_BUFFER_SIZE];
    void * ptr;
    int offset;

    int count = 0;
    for (int i=0; fmt[i] != 0; i++) {
        if (fmt[i] == '%') {
            switch (fmt[++i]) {
                // String
                case 's':
                    ptr = (void *) va_arg(args, char *);
                    offset = readToBlank(input, index);
                    strncpy(ptr, input + index, offset);
                    index += (offset + 1);
                    count++;
                    break;
                // Integer
                case 'd':
                    ptr = (void *) va_arg(args, int *);
                    offset = readToBlank(input, index);
                    strncpy(buffer, input + index, offset);
                    *(int *)ptr = stringToInt(buffer);
                    index += (offset + 1);
                    count++;
                    break;
                // Character
                case 'c':
                    if(input[index] != 0) {
                        *(char *)va_arg(args, char *) = input[index];
                        index++;
                    }
                    count++;
                    break;
                // No special format found
                default:
                    index++;
                    break;
            }
        }
    }
    va_end(args);
    return count;
}

int readFromKeyboard(char * buffer) {
    int i=0;
    char c;
    do{
        c = getChar();
        buffer[i++] = c;
    }while (c != ' ' && c != '\t' && c != '\n' && i < BUFFER_DIM);
    return i;
}


char *strncpy(char *dest, const char *src, int count)
{
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}

char *strchr(char * input, int character){
    for(int i= 0;input[i] != 0; i++){
        if(input[i] == character){
            return (char *) &input[i];
        }
    }
    return NULL;
}

//static unsigned long int next = 1;


int ownRand(int from, int to) {
  unsigned long long ticks = (unsigned long long) sys_ticks();
    return (from + (ticks % to));
}

int atoi(const char * input) {
    int result = 0;
    int signMultiplier = 1;

    if (input[0] == '-') {
        signMultiplier = -1;
        input++;
    }

    for (int i = 0; input[i] != '\0' && input[i] >= '0' && input[i] <= '9'; i++) {
        result = result * 10 + (input[i] - '0');
    }

    return result * signMultiplier;
}

char* itoa(int num, char* str) {
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