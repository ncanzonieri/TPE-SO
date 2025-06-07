#include <commands.h>
#include <syscalls.h>
#include <library.h>

void ps() {
    ProcessInfo* processes = (ProcessInfo*) sys_showProcesses();
    char* status[]={ "READY     ", "RUNNING   ", "BLOCKED   ", "TERMINATED" };
    sys_write(STDOUT_FD, "Status    ;FG;PID;PPID;Name    ;Prio;StackBase\n", 48, 0xffffff);
    for (int i = 0; processes[i].pid != -1; i++) {
        printf("%s;%d ;%s%d ;%s%d  ;%s;%d   ;%p \n", 
               status[processes[i].status], 
               processes[i].foreground, 
               processes[i].pid > 9 ? "" : "0",
               processes[i].pid,
               processes[i].pPid > 9 ? "" : "0", 
               processes[i].pPid, 
               processes[i].name, 
               processes[i].priority, 
               (uint64_t*)processes[i].stackBase);
    }
}