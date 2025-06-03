#include "../include/process.h"
#include "../include/scheduler.h"

static void exit_process(int ret, unsigned int pid) {
	Process process = getProcess(pid);
	process->retValue = ret;
	killProcess(pid);
	_yield();
}
//wrapper de _start
void runProcessWrapper(ProcessEntry func, char **argv, uint64_t argc) {
    int newArgc = argCount(argv);
    int ret = func(newArgc, argv);
    exit_process(ret, getPid());
}

void initProcess(Process process, char* name, uint64_t pid, uint64_t ppid, uint8_t priority, char foreground, char** argv, int argc, ProcessEntry func){
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