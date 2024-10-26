
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
