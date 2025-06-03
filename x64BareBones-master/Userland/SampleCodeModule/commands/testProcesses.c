#include <test_utils.h>
#include <commands.h>
#include <syscalls.h>
#include <stdio.h>

void testProcesses(){
    char* argv[1];
    argv[0] = "16";
    sys_createProcess("testprocess", 1, 0, &test_processes, argv, 1);
}

void testPriorities(){
    //char* argv[1];
    //argv[0] = "";
    sys_createProcess("testPriority", 1, 0, &test_prio, NULL, 0);
}

