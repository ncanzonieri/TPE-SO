// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <syscalls.h>

int divx0(int argc, char *argv[]) {
    int a, b, c;
    a = 0;
    b = 1;
    c = b / a;
    a = c;
    return 0;
}


int invalid(int argc, char *argv[]){
    invalidOperation();
    // codOpInvalidASM(); // se hace en asm
    return 0;
}
