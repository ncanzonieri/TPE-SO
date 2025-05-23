#include "scheduler.h"

//typedef int (*main_function)(uint64_t argc, char **argv);
static char** copyArgs(char** argv);


static int initialized = 0;

Sched initScheduler() {
	Sched scheduler = (Sched) SCHEDULER_ADDRESS;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        scheduler->processes[i].status = TERMINATED;
    }
    scheduler->currentPid = 1;
    scheduler->nextPid = 1;
    scheduler->processCount = 0;
	scheduler->availableIndex = 0;
	scheduler->currIndex = 0;
    //scheduler->quantumRemaining = QUANTUM;
    //scheduler->foreground = 0;
    //scheduler->killForeground = 0;
    initialized = 1;
	return scheduler;
}

Sched getScheduler() {
	return (Sched) SCHEDULER_ADDRESS;
}


int64_t createProcess(char* name, uint8_t priority, void (*entry_point)(int, char**), char** argv, int argc, int16_t fds[]) {
	Sched scheduler = getScheduler();
    int16_t availableIndex = scheduler->availableIndex;
	if (scheduler->processCount == MAX_PROCESSES || availableIndex == -1) {
		return -1;
	}
    scheduler->processCount++;
    Process process = &scheduler->processes[availableIndex];
	process->status = READY;
	updateAvailableIndex(scheduler);
	uint64_t pid, pPid;
	if (scheduler->nextPid == INIT_PID) {
		pid = INIT_PID;
		pPid = pid;
		scheduler->nextPid++;
	} else {
		pid = scheduler->nextPid++;
		pPid = getPid();
	}
    char** newArgv = copyArgs(argv/*, argc*/);

    init_process(process, name, pid, pPid, priority, newArgv, argc, entry_point, fds);
	Process parent = getProcess(pPid);
    parent->wPid++;
	return pid;
}

/*
static char** copyArgs(char** argv, uint64_t argc) {
    uint64_t totalLength = 0;
    int argLengths[argc];
    for (int i = 0; i < argc; i++) {
        argLengths[i] = strlen(argv[i]) + 1;
        totalLength += argLengths[i];
    }
    char** newArgv = (char**)myMalloc(totalLength + sizeof(char*) * (argc + 1));
    char* stringPos = (char*)newArgv + (sizeof(char*) * (argc + 1));
    for (int i = 0; i < argc; i++) {
        newArgv[i] = stringPos;
        memcpy(stringPos, argv[i], argLengths[i]);
        stringPos += argLengths[i];
    }
    newArgv[argc] = NULL;
    return newArgv;
}
*/ // LA DE ABAJO APROVECHA FUNCION DE LIB NUESTRA
static char** copyArgs(char** argv) {
    int argc = argCount(argv);
    uint64_t totalLength = 0;
    int argLengths[argc];

    for (int i = 0; i < argc; i++) {
        argLengths[i] = strlen(argv[i]) + 1;
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

void updateAvailableIndex(Sched scheduler) {
	int16_t availableIndex = -1;
	for (int i = 0; i < MAX_PROCESSES; i++) {
		if (scheduler->processes[i].status == TERMINATED) {
			availableIndex = i;
			break;
		}
	}
	scheduler->availableIndex = availableIndex;
}
//hay que hacer en el asm, el guardado del contexto para luego recuperarlo y hacer una syscall de este
