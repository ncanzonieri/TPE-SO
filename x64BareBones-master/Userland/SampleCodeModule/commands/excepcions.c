#include <commands.h>

void divx0() {
    int a, b, c;
    a = 0;
    b = 1;
    c = b / a;
    a = c;

}


void invalid(){
    invalidOperation();
    // codOpInvalidASM(); // se hace en asm
}
