#include <unistd.h>
#include <test_utils.h>
#include <commands.h>
#include <syscalls.h>

void testMM(){
    uint64_t argc = 1;
    char *argv[1];
    argv[0] = "1048576"; // 1MB
    if(test_mm(argc, argv)==-1)
        sys_write(STDOUT_FD, "test_mm ERROR\n", 14, 0x00FF0000);
}