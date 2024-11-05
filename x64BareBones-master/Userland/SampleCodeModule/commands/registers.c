#include <commands.h>
#include <syscalls.h>
#include <library.h>
#define REGS_AMOUNT 20 
#define ERROR 

static char * regsNames[] = {  
    "RAX   ", "RBX   ", "RCX   ", "RDX   ", "RSI   ", "RDI   ", "RBP   ",
    "R8    ", "R9    ", "R10   ", "R11   ", "R12   ", "R13   ", "R14   ",
    "R15   ", "RSP   ", "RIP   ", "SS    ", "CS    ", "RFLAGS"
};

static void hexaToAscii(uint64_t num, char* buffer){
    char rest;
    for(int i=0; i<16; i++, num/=16){
        rest=num%16;
        buffer[17-i]= (rest < 10 )? (rest + '0') : (rest - 10 + 'A');
    }
    buffer[1]='x';
    buffer[0]='0';
}

void registers(){
    uint64_t r[REGS_AMOUNT];
    int flag = sys_getRegisters(r);
    if( !flag ){
        clearIfNotEnoughSpace(1);
        sys_write(STDOUT_FD,"error: wrong registers upload\n",31,0xFFFFFF);

        return;
    }
    char buffer[18];
    clearIfNotEnoughSpace(REGS_AMOUNT);
    for( int i=0; i<REGS_AMOUNT && flag; i++ ){
        //printf("%s: %s\n", regsNames[i], r[i]);
        sys_write(STDOUT_FD,regsNames[i],14,0xFFFFFF);
        hexaToAscii(r[i],buffer);
        sys_write(STDOUT_FD,buffer,18,0xFFFFFF);
        sys_write(STDOUT_FD,"\n",2,0xffffff);
    }
return;
}