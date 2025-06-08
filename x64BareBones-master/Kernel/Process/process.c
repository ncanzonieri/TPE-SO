#include "../include/process.h"
#include "../include/scheduler.h"
#include "../include/pipes.h"

static void exit_process(int ret) {
    Sched scheduler = getScheduler();
    int64_t pid = getPid();
    int64_t ppid = scheduler->processes[pid].pPid;
	Process process = getProcess(pid);
	process->retValue = ret;
    process->status = TERMINATED;
    scheduler->processCount--;
    myFree(process->stackBase);
    myFree(process->argv);
    if(process->write != STDOUT) {
        sendEOF(process->write);
    }
    if(ppid != INIT_PID) {
        Process parent = getProcess(ppid);
        if (parent && parent->status == BLOCKED && parent->wPid == pid) {
            parent->status = READY;
        }
    }
	//killProcess(pid);
	_yield();
}
//wrapper de _start
void runProcessWrapper(ProcessEntry func, char **argv, uint64_t argc) {
    int newArgc = argCount(argv);
    int ret = func(newArgc, argv);
    exit_process(ret);
}

void initProcess(Process process, char* name, uint64_t pid, uint64_t ppid, uint8_t priority, char foreground, char** argv, int argc, ProcessEntry func, int * fds){
    myStrncpy(process->name, name, sizeof(process->name));
    process->pid = pid;
	process->pPid = ppid;
	process->priority = priority;
	process->timesToRun = priority;
    process->argc = argc;
    process->argv = argv;
    process->foreground = foreground;
    process->stackBase = myMalloc(STACK_SIZE);
    process->status = READY;
    process->read = fds[0];
    process->write = fds[1];
    if (process->stackBase == NULL) {
        //error
        myFree(process->stackBase);
        return;
    }
    void *stackEnd = (void *) ((uint64_t) process->stackBase + STACK_SIZE);
    process->stackPtr = _initialize_stack_frame(&runProcessWrapper, func, stackEnd, (void *) process->argv);
}

uint64_t getPid() {
    Sched scheduler = getScheduler();
    return scheduler->currentPid;
}

char* processInfo(Process process) {
    static char status[16];
    myStrncpy(status, "", 16);
    
    switch (process->status) {
        case READY:
            myStrncpy(status, "REA", 14);
            break;
        case RUNNING:
            myStrncpy(status, "RUN", 14);
            break;
        case BLOCKED:
            myStrncpy(status, "BLO", 14);
            break;
        case TERMINATED:
            myStrncpy(status, "TER", 14);
            break;
        default:
            myStrncpy(status, "UNKNOWN", 14);
            return status;
    }
    if (process->priority == 4) {
        myStrncpy(status + myStrlen(status), " - HIGH - ", 14 - myStrlen(status));
    }
    else if (process->priority == 3) {
        myStrncpy(status + myStrlen(status), " - MEDH - ", 14 - myStrlen(status));
    }
    else if (process->priority == 2) {
        myStrncpy(status + myStrlen(status), " - MEDL - ", 14 - myStrlen(status));
    }
    else if (process->priority == 1) {
        myStrncpy(status + myStrlen(status), " - LOW - ", 14 - myStrlen(status));
    }
    if (process->foreground) {
        myStrncpy(status + myStrlen(status), "F", 14 - myStrlen(status));
    }
    else if (process->pid == INIT_PID) {
        myStrncpy(status + myStrlen(status), "I", 14 - myStrlen(status));
    }
    return status;
}

int64_t waitChildren(uint64_t pid) {
    Sched scheduler = getScheduler();
    if (scheduler == NULL || pid >= MAX_PROCESSES || pid == INIT_PID) {
        return -1;
    }
    if(scheduler->processes[pid].pid == -1) {
        return -1;
    }
    if(scheduler->currentPid != scheduler->processes[pid].pPid) {
        return -1;
    }
    if(scheduler->processes[pid].status != TERMINATED) {
        scheduler->processes[scheduler->currentPid].wPid = pid;
        blockProcess(scheduler->currentPid);
    }
    scheduler->processes[scheduler->currentPid].wPid = -1;
    int64_t retValue = scheduler->processes[pid].retValue;
    return retValue;

    // Process child = getProcess(pid);
    // if (child == NULL) {
    //     return -1;
    // }
    // Process parent = getProcess(child->pPid);
    
    // if (child->status != TERMINATED) {
    //     parent->wPid = pid;
    //     blockProcess(parent->pid);
    //     //_yield();
    // }
    // int8_t retValue = child->retValue;
    // child->status = TERMINATED;
    // scheduler->processCount--;
    // return retValue;
}