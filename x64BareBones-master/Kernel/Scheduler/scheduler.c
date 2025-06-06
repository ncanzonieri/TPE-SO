#include "scheduler.h"

static char** copyArgs(char** argv, int argc);
static void updateAvailableIndex(Sched scheduler);
static int initialized = 0;
static ProcessInfo processList[MAX_PROCESSES+1];

Sched initScheduler() {
	Sched scheduler = (Sched) SCHEDULER_ADDRESS;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        scheduler->processes[i].status = TERMINATED;
    }
    scheduler->currentPid = 0; //1
    scheduler->nextPid = 0; //1
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


int64_t createProcess(char* name, uint8_t priority, char foreground, ProcessEntry func, char** argv, int argc) {
	Sched scheduler = getScheduler();
    int16_t availableIndex = scheduler->availableIndex;
	if (scheduler->processCount == MAX_PROCESSES || availableIndex == MAX_PROCESSES) {
		return -1;
	}
    scheduler->processCount++;
    Process process = &scheduler->processes[availableIndex];
	//process->status = READY;
	// updateAvailableIndex(scheduler);
	
    uint64_t pPid;
    if(scheduler->availableIndex == INIT_PID) {
		pPid = INIT_PID;
		// scheduler->nextPid++;
	} else {
		pPid = scheduler->currentPid;
	}
	// scheduler->currentPid = availableIndex;
    char** newArgv = copyArgs(argv, argc);
    initProcess(process, name, availableIndex, pPid, priority, foreground, newArgv, argc, func);

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
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (scheduler->processes[i].pid == pid && scheduler->processes[i].status != TERMINATED) {
            if (scheduler->processes[i].status == BLOCKED) {
                return 0;
            }
            scheduler->processes[i].status = BLOCKED;
            return 1;
        }
    }
    return 0;
}

uint16_t unblockProcess(uint64_t pid) {
    Sched scheduler = getScheduler();
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (scheduler->processes[i].pid == pid && scheduler->processes[i].status == BLOCKED) {
            scheduler->processes[i].status = READY;
            return 1;
        }
    }
    return 0;
}

uint64_t killProcess(uint64_t pid) {
    Sched scheduler = getScheduler();
    if (pid <= INIT_PID || pid >= MAX_PROCESSES || scheduler->processes[pid].status == TERMINATED) {
        //error
        return 0;
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

    if (scheduler->currentPid == pid) {
        _yield();
    }
    return 1;
}

uint64_t changePriority(uint64_t pid, uint8_t newPriority) {
    Sched scheduler = getScheduler();
    Process process = NULL;

    if (pid == INIT_PID) {
		//error
        return 0;
    }

    for (int i = 0; i < MAX_PROCESSES; i++) {
        // if (scheduler->processes[i].pid == pid && scheduler->processes[i].status != TERMINATED) {
        //     process = &scheduler->processes[i];
        //     break;
        // }
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
        return stackPtr;  // If the scheduler is not initialized, return the current stack pointer
    }
    Sched scheduler = getScheduler();
    if(scheduler->processCount == 0) {
        return stackPtr;  // If there are no processes, return the current stack pointer
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
    
    /*
    if (!initialized) {     return stackPtr;    }  //chequear 

    Process currentProcess = &scheduler->processes[scheduler->currentPid];

    if (currentProcess->status == TERMINATED) {
        scheduler->currIndex = (scheduler->currIndex + 1) % MAX_PROCESSES;
        return stackPtr;
    }

    if (scheduler->quantumRemaining <= QUANTUM) {   currentProcess->stackPtr = stackPtr;    }

    currentProcess = updateQuantum(stackPtr);
    scheduler->currentPid = currentProcess->pid;

    return currentProcess->stackPtr;
    */
}

// no se usa
Process updateQuantum(void *stackPtr) {
    Sched scheduler = getScheduler();
    Process currentProcess = &scheduler->processes[scheduler->currentPid];

    if (scheduler->quantumRemaining == 0 || currentProcess->status == BLOCKED || currentProcess->timesToRun == 0) {
        if (currentProcess->status == RUNNING) {    currentProcess->status = READY; }

        if (currentProcess->status != BLOCKED && currentProcess->timesToRun > 0) {  currentProcess->timesToRun--;   }
        
        currentProcess->stackPtr = stackPtr;

        while (currentProcess->status != READY || currentProcess->timesToRun == 0) {
            scheduler->currIndex = (scheduler->currIndex + 1) % MAX_PROCESSES;
            currentProcess = &scheduler->processes[scheduler->currIndex];

            if (currentProcess->timesToRun == 0 && currentProcess->status == READY) {
                currentProcess->timesToRun = currentProcess->priority;
            }
        }

        scheduler->quantumRemaining = QUANTUM;
    } else {
        scheduler->quantumRemaining--;
    }

    currentProcess->status = RUNNING;

    return currentProcess;
}

static char** copyArgs(char** argv, int argc) {
    // int argc = argCount(argv);
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

static void updateAvailableIndex(Sched scheduler) {
	int16_t availableIndex = -1;
	for (int i = 0; i < MAX_PROCESSES; i++) {
		if (scheduler->processes[i].status == TERMINATED) {
			availableIndex = i;
			break;
		}
	}
	scheduler->availableIndex = availableIndex;
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
            processList[count++] = process;
        }
    }
    ProcessInfo emptyProcess;
    emptyProcess.pid = -1;
    processList[count] = emptyProcess; // Null-terminate the list
   

    
    // newLine();
    // for (int i = 0; i < MAX_PROCESSES; i++) {
    //     if (scheduler->processes[i].status != TERMINATED) {
            
    //         printString(0xFFFFFF, "        ");
    //         printString(0xFFFFFF, processInfo(&scheduler->processes[i]));
    //         printString(0xFFFFFF, "        ");
    //         printHex((uint64_t)scheduler->processes[i].stackPtr);
            
            
    //         newLine();
    //     }
    // }
    // newLine();

    return processList;
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

// uint64_t updatePriority(uint64_t pid, uint64_t newPriority){
//     Sched scheduler = getScheduler();
//     if(pid >= MAX_PROCESSES || scheduler->processes[pid].status == TERMINATED || newPriority > MAX_PRIORITY || newPriority < MIN_PRIORITY){
//         return -1;
//     }
//     scheduler->processes[pid].priority = newPriority;
//     //scheduler->processes[pid].timesToRun = ;
//     //return 0;

// }