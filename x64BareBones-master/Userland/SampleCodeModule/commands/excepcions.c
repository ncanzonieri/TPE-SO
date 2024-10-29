#include <commands.h>


extern void invalidOperation();




void divx0() {
    int a, b, c;
    a = 0;
    b = 1;
    c = b / a;
    a = c;
}


void codOpInvalid(){
    invalidOperation();
    // codOpInvalidASM(); // se hace en asm
}
