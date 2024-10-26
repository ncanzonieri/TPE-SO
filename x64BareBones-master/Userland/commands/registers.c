#include <commands.h>

#define MAX_REGIS 16

static char * registers[] = {  //mismo orden que en FILLSNAPSHOT (interrupts.asm)
    "RAX   ", "RBX   ", "RCX   ", "RDX   ", "RSI   ", "RDI   ", "RBP   ",
    "R8    ", "R9    ", "R10   ", "R11   ", "R12   ", "R13   ", "R14   ",
    "R15   ", "RSP   ", "RIP   "
};

extern void getRegisters(int registers[MAX_REGIS]);

void registers() {
    int registersCopy[MAX_REGIS];
    getRegisters(registersCopy); // Obtener los valores de los registros

    printf("----- Registers -----\n");
    for (int i = 0; i < MAX_REGIS; i++) {
        printf("%s: %d\n", registers[i], registersCopy[i]); // Imprimir el nombre del registro y su valor
    }
}

// ----- chequear que onda, la hice asi no mas y rapido

