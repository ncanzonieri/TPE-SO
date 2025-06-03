#include <commands.h>
#include <syscalls.h>
#include <library.h>

void ps() {
    ProcessInfo* processes = (ProcessInfo*) sys_showProcesses();
    char* status[]={ "READY", "RUNNING", "BLOCKED", "TERMINATED" };
    sys_write(STDOUT_FD, "Status;Foreground;PID;PPID;Name;Priority;StackBase\n", 52, 0xffffff);
    for (int i = 0; processes[i].pid != -1; i++) {
        printf("%s ;%d ;%d ;%d ;%s ;%d ;%p\n", 
               status[processes[i].status], 
               processes[i].foreground, 
               processes[i].pid, 
               processes[i].pPid, 
               processes[i].name, 
               processes[i].priority, 
               (uint64_t*)processes[i].stackBase);
    }
}