#include <commands.h>
#include <syscalls.h>
#include <library.h>
#define REGS_AMOUNT 17 
#define ERROR 

static char * regsNames[] = {  
    "RAX   ", "RBX   ", "RCX   ", "RDX   ", "RSI   ", "RDI   ", "RBP   ",
    "R8    ", "R9    ", "R10   ", "R11   ", "R12   ", "R13   ", "R14   ",
    "R15   ", "RSP   ", "RIP   "
};

static void hexaToAscii(uint64_t num, char* buffer){
    char rest;
    for(int i=0; i<64; i++, num/=16){
        rest=num%16;
        buffer[65-i]= (rest < 10 )? (rest + '0') : (rest - 10 + 'A');
    }
    buffer[1]='x';
    buffer[0]='0';
}

void registers(){
    uint64_t r[REGS_AMOUNT];
    int flag = sys_getRegisters(r);
    if( flag != 1 ){
        sys_write(STDOUT_FD,"error: wrong registers upload",14,0xFFFFFF);

        return;
    }
    char buffer[66];
    for( int i=0; i<REGS_AMOUNT && flag; i++ ){
        //printf("%s: %s\n", regsNames[i], r[i]);
        sys_write(STDOUT_FD,regsNames[i],14,0xFFFFFF);
        hexaToAscii(r[i],buffer);
        sys_write(STDOUT_FD,buffer,66,0xFFFFFF);
        sys_write(STDOUT_FD,"\n",2,0xffffff);
    }
return;
}

/*static int inforegCommand(int argc, char * argv[]) {
    uint64_t regs[REGS_AMOUNT];
    int ok = _sys_getRegisters(regs);
    if(!ok) {
        printError("inforeg", "Registers are not updated. Use CTRL + R to update.", NULL);
        return ERROR;
    }
    char changed = 0;
    if(scale == 3) {
        changed = 1;
        setFontScale(2);
    }
    for(int i=0; i<REGS_AMOUNT; i += 2) {
        printStringColor(regNames[i], COMMAND_SECONDARY_COLOR);
        printf(": %x\t", regs[i]);
        if(i < (REGS_AMOUNT - 1)) {
            printStringColor(regNames[i + 1], COMMAND_SECONDARY_COLOR);
            printf(": %x\n", regs[i + 1]);
        }
        else
            putchar('\n');
    }
    if(changed) {
        setFontScale(3);
    }
    return OK;
}*/


// ---------------- NUESTRO REGIS -------------------
// sys_getRegisters: syscall 0x09
