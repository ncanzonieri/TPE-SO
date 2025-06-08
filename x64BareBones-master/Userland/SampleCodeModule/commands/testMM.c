#include <unistd.h>
#include <test_utils.h>
#include <commands.h>
#include <syscalls.h>
#define WHITE 0xffffff

int testMM(int argc, char *argv[]) {
    if(test_mm(argc, argv) == -1){
        sys_write(STDOUT_FD, "Error al ingresar argumentos: testMM espera <max_memory> positivo.\n",68, WHITE);
        return 1;
    }
    return 0;
}