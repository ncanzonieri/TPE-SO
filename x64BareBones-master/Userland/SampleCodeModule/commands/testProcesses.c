#include <test_utils.h>
#include <commands.h>
#include <syscalls.h>
#include <stdio.h>

void testProcesses(){
    char* argv[1];
    argv[0] = "16";
    if(sys_createProcess("testprocess", 1, 0, &test_processes, argv, 1) == -1){
        printf("Error al inicializar testProcesses\n");
    };
}

void testPriorities(){
    //char* argv[1];
    //argv[0] = "";
    //printf("pre testPriority %d\n", sys_getPid());
    if(sys_createProcess("testPriority", 1, 0, &test_prio, NULL, 0) == -1){
        printf("Error al inicializar testPriority\n");
    };
    //printf("post testPriority %d\n", sys_getPid());
}

