#include <stdint.h>
#include "../Process/process.h"
#include <stddef.h>
#include "../include/lib.h"
#include "../include/MemoryManagerADT.h"

#define SCHEDULER_ADDRESS 0x600000
#define INIT_PID 1

#define MAX_PROCESSES 32
#define STACK_SIZE 4096


//ESTRUCTURA TENTATIVA SCHEDULER
typedef struct Scheduler {
	PCB processes[MAX_PROCESSES];
    uint64_t currentPid;
	uint64_t nextPid;
	uint64_t processCount;
	int16_t availableIndex;
	int16_t currIndex;
	//int8_t quantumRemaining;
    //uint64_t foregroundPid;       // Foreground process PID
    //uint8_t killForeground;       // Flag to kill foreground process
} Scheduler;

typedef struct Scheduler* Sched;

//uint64_t createProcess(char *name, Priority priority, char *argv[], int argc, main_function rip, const int16_t fds[]);
Sched initScheduler();
Sched getScheduler();
int64_t createProcess(char* name, uint8_t priority, void (*entry_point)(int, char**), char** argv, int argc, int16_t fds[]);
Process getProcess(uint64_t pid);