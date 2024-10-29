/* sampleCodeModule.c */


#include <powershell.h>
#include <library.h>
#include <syscalls.h>

#define WHITE 0x00FFFFFF
char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;



int main(){
    welcome();
    getCommands();
    while(1){
        char c;
        char buffer[256];
        int index=0;
        while( c = getChar() != '\n'){
            buffer[index++] = c;
            putChar(c,WHITE);
        }
    }
    return 0;
}

