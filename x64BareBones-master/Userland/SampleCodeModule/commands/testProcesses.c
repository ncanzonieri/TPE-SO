#include <test_utils.h>
#include <commands.h>
#include <syscalls.h>

void testProcesses(){
    char* argv[1];
    argv[0] = "16";
    sys_createProcess("testprocess", 1, 0, &test_processes, argv, 1);
}