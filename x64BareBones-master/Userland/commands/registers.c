#include <commands.h>

#define MAX_REGIS 16

static const char * registerConsts[MAX_REGIS] = {
        "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "RSP"
};

extern void getRegisters(int registers[MAX_REGIS]);

void registers() {
    int registers[MAX_REGIS];
    getRegisters(registers); // Obtener los valores de los registros

    printf("----- Registers -----\n");
    for (int i = 0; i < MAX_REGIS; i++) {
        printf("%s: %d\n", registerConsts[i], registers[i]); // Imprimir el nombre del registro y su valor
    }
}

// ----- chequear que onda, la hice asi no mas y rapido