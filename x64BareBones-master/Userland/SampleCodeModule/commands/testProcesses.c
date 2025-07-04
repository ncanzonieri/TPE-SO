// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_utils.h>
#include <commands.h>
#include <syscalls.h>
#include <stdio.h>

int testProcesses(int argc, char *argv[]){
    if(test_processes(argc,argv) == -1){
        printf("Error al inicializar testProcesses\n");
        return 1;
    };
    return 0;
}

int testPriorities(int argc, char *argv[]){
    test_prio();
    return 0;
}

int testSync(int argc, char *argv[]){
    if(test_sync(argc, argv) == -1){
        printf("Error al inicializar testSync\n");
        return 1;
    }
    return 0;
}