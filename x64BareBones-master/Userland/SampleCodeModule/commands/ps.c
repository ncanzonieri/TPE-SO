#include <commands.h>
#include <syscalls.h>

void ps() {
    sys_showProcesses();
}