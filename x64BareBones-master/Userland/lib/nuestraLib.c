#include <stdint.h>
#include <stdarg.h>
#define DIM 256
#define STDOUT 1
#define WHITE 0x00FFFFFF


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

static int strConcat(char *str1, char *str2){
    int i = strlen(str1);
    int j = 0;
    while(str2[j] != '\0'){
        str1[i] = str2[j];
        i++;
        j++;
    }
    return i;
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
static int intToString(int num, char *str) {
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


static int strConcat(char *str1, char *str2){
    int i = strlen(str1);
    int j = 0;
    while(str2[j] != '\0'){
        str1[i] = str2[j];
        i++;
        j++;
    }
    return i;
}

static int stringToInt(char * num){
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
    return putString(str, WHITE);
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
                    *(int *)ptr = atoi(buffer);
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

static int readFromKeyboard(char * buffer) {
    int i=0;
    char c;
    while (c != ' ' && c != '\t' && c != '\n' && i < BUFFER_DIM) {
        c = getChar();
        buffer[i++] = c;
    }
    return i;
}
