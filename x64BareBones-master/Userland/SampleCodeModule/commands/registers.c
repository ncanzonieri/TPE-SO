#include <commmands.h>

static char * regsNames[] = {  
    "RAX   ", "RBX   ", "RCX   ", "RDX   ", "RSI   ", "RDI   ", "RBP   ",
    "R8    ", "R9    ", "R10   ", "R11   ", "R12   ", "R13   ", "R14   ",
    "R15   ", "RSP   ", "RIP   "
};

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



static void printError(char * command, char * message, char * usage) {
    printf("%s: ", command);
    printStringColor("error: ", ERROR_SECONDARY_COLOR);
    printStringColor(message, ERROR_PRIMARY_COLOR);
    if(usage != NULL)
        printf("\nUsage: %s\n", usage);
    else
        putchar('\n');
}

void registers(){
    int 
    for( int i=0; i<REGS_AMOUNT; i++ ){
    
    }

}