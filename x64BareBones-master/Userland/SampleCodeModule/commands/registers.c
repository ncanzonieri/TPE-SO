#include <commands.h>
#include <syscalls.h>
#include <library.h>
#define REGS_AMOUNT 17 
static char * regsNames[] = {  
    "RAX   ", "RBX   ", "RCX   ", "RDX   ", "RSI   ", "RDI   ", "RBP   ",
    "R8    ", "R9    ", "R10   ", "R11   ", "R12   ", "R13   ", "R14   ",
    "R15   ", "RSP   ", "RIP   "
};
/*
static int inforegCommand(int argc, char * argv[]) {
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
}

*/

// ---------------- NUESTRO REGIS -------------------
// sys_getRegisters: syscall 0x09

void registers(){
    uint64_t r[REGS_AMOUNT];
    int flag = sys_getRegisters(r);
    if( flag != 1 ){
        printf("error: wrong registers upload");
        return;
    }
    for( int i=0; i<REGS_AMOUNT && flag; i++ ){
        printf("%s: %s\n", regsNames[i], r[i]);
    }
return;
}