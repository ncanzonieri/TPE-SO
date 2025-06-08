#include <commands.h>
#include <library.h>
#include <syscalls.h>
#define WHITE 0xffffff
#define GREEN 0x66FF66
#define STDOUT_FD 1

#define INSTRUCTIONS 16
int help(int argc, char *argv[]){
    if(argc > 0){
        int i;
        for(i = 0; i < INVALID_OPERATION; i++){
            if(strcmp(argv[0], commands[i].name) == 0){
                clearIfNotEnoughSpace(1);
                printf("%s: %s\n", commands[i].name, commands[i].description);
                return 0;
            }
        }
    }
    clearIfNotEnoughSpace(INVALID_OPERATION);
    sys_write(STDOUT_FD, "------------------- Help -------------------\n" , 46, GREEN);
    for(int i = 0; i<INVALID_OPERATION; i++){
        printf("%s: %s\n", commands[i].name, commands[i].description);
    }
    return 0;
}