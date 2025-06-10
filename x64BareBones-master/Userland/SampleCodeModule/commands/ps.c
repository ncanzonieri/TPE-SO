// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <syscalls.h>
#include <library.h>
#define MAX_PROCESSES 20

int ps(int argc, char* argv[]) {
    ProcessInfo* processes = (ProcessInfo*) sys_showProcesses();
    char* status[]={ "READY     ", "RUNNING   ", "BLOCKED   ", "TERMINATED" };
    sys_write(STDOUT_FD, "\nStatus    ;FG;PID;PPID;Prio;StackPtr;StackBase;Name\n", 54, 0xffffff);
    for (int i = 0; processes[i].pid < MAX_PROCESSES; i++) {
        printf("%s;%d ;%s%d ;%s%d  ;%d   ;%p;%p ;%s\n", 
            status[processes[i].status], 
            processes[i].foreground, 
            processes[i].pid > 9 ? "" : "0",
            processes[i].pid,
            processes[i].pPid > 9 ? "" : "0", 
            processes[i].pPid, 
            processes[i].priority, 
            (uint64_t*)processes[i].stackPtr,
            (uint64_t*)processes[i].stackBase,
            processes[i].name);
    }
    return 0;
}