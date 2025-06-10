// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "scheduler.h"
#include "../include/pipes.h"

static char** copyArgs(char** argv, int argc);
static int initialized = 0;
static ProcessInfo processList[MAX_PROCESSES+1];

Sched createScheduler() {
	Sched scheduler = (Sched) SCHEDULER_ADDRESS;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        scheduler->processes[i].status = TERMINATED;
    }
    scheduler->currentPid = 0;
    scheduler->nextPid = 0;
    scheduler->processCount = 0;
	scheduler->availableIndex = 0;
	scheduler->currIndex = 0;
    scheduler->quantumRemaining = QUANTUM;
    initialized = 1;
	return scheduler;
}

Sched getScheduler() {
	return (Sched) SCHEDULER_ADDRESS;
}


int64_t createProcess(char* name, uint8_t priority, char foreground, ProcessEntry func, char** argv, int argc, int * fds) {
	Sched scheduler = getScheduler();
    int16_t availableIndex = scheduler->availableIndex;
	if (scheduler->processCount == MAX_PROCESSES || availableIndex == MAX_PROCESSES) {
		return -1;
	}
    scheduler->processCount++;
    Process process = &scheduler->processes[availableIndex];
    uint64_t pPid;
    if(scheduler->availableIndex == INIT_PID) {
		pPid = INIT_PID;
	} else {
		pPid = scheduler->currentPid;
	}
    char** newArgv = copyArgs(argv, argc);
    initProcess(process, name, availableIndex, pPid, priority, foreground, newArgv, argc, func, fds);

    for(uint64_t pid=0; pid < MAX_PROCESSES; pid++) {
        if(scheduler->processes[pid].status == TERMINATED) {
            scheduler->availableIndex = pid;
            break;
        }
    }

	return process->pid;
}

uint8_t setStatus(uint8_t newStatus) {
    Sched scheduler = getScheduler();
    Process process = &scheduler->processes[scheduler->currIndex];
    pStatus oldStatus = process->status;
    if (newStatus == TERMINATED || newStatus == RUNNING || oldStatus == TERMINATED) {	return -1;	}
    process->status = newStatus;
    if (newStatus == oldStatus) {   return oldStatus;   }

    return newStatus;
}

uint16_t blockProcess(uint64_t pid) {
    Sched scheduler = getScheduler();
    if (scheduler->processes[pid].status != TERMINATED) {
        if (scheduler->processes[pid].status == BLOCKED) {
            return 0;
        }
        int flag = scheduler->processes[pid].status == RUNNING;
        scheduler->processes[pid].status = BLOCKED;
        if(flag) _yield();
        return 1;
    }
    return 0;
}

uint16_t unblockProcess(uint64_t pid) {
    Sched scheduler = getScheduler();
    if (scheduler->processes[pid].status == BLOCKED) {
        scheduler->processes[pid].status = READY;
        return 1;
    }
    return 0;
}

uint64_t killProcess(uint64_t pid) {
    Sched scheduler = getScheduler();
    if (pid <= INIT_PID + 1 || pid >= MAX_PROCESSES || scheduler->processes[pid].status == TERMINATED) {
        return 0;
    }
    Process process = &scheduler->processes[pid];
    Process parent = &scheduler->processes[process->pPid];
    

    if (parent != NULL && parent->status == BLOCKED &&
		parent->wPid == process->pid) {
		unblockProcess(parent->pid);
	}
    
    scheduler->processes[pid].status = TERMINATED;
    scheduler->processCount--;
    myFree(scheduler->processes[pid].stackBase);
    myFree(scheduler->processes[pid].argv);
    
    for (int j = 0; j < MAX_PROCESSES; j++) {
        if (scheduler->processes[j].pPid == pid && scheduler->processes[j].status != TERMINATED) {
            scheduler->processes[j].pPid = INIT_PID;
        }
    }
    if (scheduler->availableIndex > pid) {
        scheduler->availableIndex = pid;
    }

    if(scheduler->processes[pid].write != STDOUT){
        sendEOF(scheduler->processes[pid].write);
    }

    if (scheduler->currentPid == pid) {
        _yield();
    }
    return 1;
}

uint64_t changePriority(uint64_t pid, uint8_t newPriority) {
    Sched scheduler = getScheduler();
    Process process = NULL;

    if (pid == INIT_PID) {
        return 0;
    }

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if(scheduler->processes[pid].status != TERMINATED){
            process = &scheduler->processes[pid];
            break;
        }
    }
    if (process == NULL || process->priority == newPriority) {	return 0;	}
    process->timesToRun = newPriority;
    process->priority = newPriority;
    return 1;
}

void* scheduler(void *stackPtr) {
    if(!initialized){
        return stackPtr;
    }
    Sched scheduler = getScheduler();
    if(scheduler->processCount == 0) {
        return stackPtr;
    }
    
    Process currentProcess = &scheduler->processes[scheduler->currentPid];
    if (currentProcess->status == RUNNING && --currentProcess->timesToRun > 0) {
        return stackPtr;
    }
    if(currentProcess->status == BLOCKED || currentProcess->status == RUNNING) {
        currentProcess->stackPtr = stackPtr;
    }
    if (currentProcess->status == RUNNING){
        currentProcess->status = READY;
    }
    
    do{
        scheduler->currIndex = (scheduler->currIndex + 1) % MAX_PROCESSES;
        currentProcess = &scheduler->processes[scheduler->currIndex];
    }while(currentProcess->status != READY);
    currentProcess->status = RUNNING;
    scheduler->currentPid = currentProcess->pid;
    currentProcess->timesToRun = currentProcess->priority;
    return currentProcess->stackPtr;
}


static char** copyArgs(char** argv, int argc) {
    uint64_t totalLength = 0;
    int argLengths[argc];

    for (int i = 0; i < argc; i++) {
        argLengths[i] = myStrlen(argv[i]) + 1;
        totalLength += argLengths[i];
    }

    char** newArgv = (char**)myMalloc(totalLength + sizeof(char*) * (argc + 1));
    char* stringPos = (char*)newArgv + sizeof(char*) * (argc + 1);

    for (int i = 0; i < argc; i++) {
        newArgv[i] = stringPos;
        memcpy(stringPos, argv[i], argLengths[i]);
        stringPos += argLengths[i];
    }

    newArgv[argc] = NULL;
    return newArgv;
}

Process getProcess(uint64_t pid) {
	Sched scheduler = getScheduler();
   
	for (int i = 0; i < MAX_PROCESSES; i++) {
		if (scheduler->processes[i].pid == pid) {
			return &scheduler->processes[i];
		}
	}
	return NULL;
}

ProcessInfo* showProcessesStatus() {
    Sched scheduler = getScheduler();

    int count = 0;
    for(int i = 0; i < MAX_PROCESSES; i++) {
        if (scheduler->processes[i].status != TERMINATED) {
            ProcessInfo process;
            process.pid = scheduler->processes[i].pid;
            myStrncpy(process.name, scheduler->processes[i].name, MAX_LENGTH);
            process.priority = scheduler->processes[i].priority;
            process.foreground = scheduler->processes[i].foreground;
            process.status = scheduler->processes[i].status;
            process.stackBase = scheduler->processes[i].stackBase;
            process.pPid = scheduler->processes[i].pPid;
            process.stackPtr = scheduler->processes[i].stackPtr;
            processList[count++] = process;
        }
    }
    ProcessInfo emptyProcess;
    emptyProcess.pid = -1;
    processList[count] = emptyProcess;

    return processList;
}

void getFDs(int* fds) {
    if(!initialized){
        fds[0] = STDIN;
        fds[1] = STDOUT; 
        return;
    }
    Sched scheduler = getScheduler();
	if (scheduler->currentPid <= 1){
        fds[0] = STDIN;  
        fds[1] = STDOUT;
        return;
    }

    fds[0] = scheduler->processes[scheduler->currentPid].read;
	fds[1] = scheduler->processes[scheduler->currentPid].write;
}

void killForegroundProcess() {
    if(!initialized){
        return;
    }
    Sched scheduler = getScheduler();
    for (int i = 2; i < MAX_PROCESSES; i++) {
        if (scheduler->processes[i].foreground && scheduler->processes[i].status != TERMINATED) {
            killProcess(scheduler->processes[i].pid);
        }
    }
}
