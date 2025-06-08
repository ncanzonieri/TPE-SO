#include <commands.h>
#include <syscalls.h>

int divx0(int argc, char *argv[]) {
    int a, b, c;
    a = 0;
    b = 1;
    c = b / a;
    a = c;
}


int invalid(int argc, char *argv[]){
    invalidOperation();
    // codOpInvalidASM(); // se hace en asm
}
