#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "./process.h"
#include <stddef.h>
#include "./lib.h"
#include "./videoDriver.h"
#include "./MemoryManagerADT.h"

#define SCHEDULER_ADDRESS 0x60000
#define INIT_PID 0
#define QUANTUM 1
#define MAX_PROCESSES 20 //32
#define STACK_SIZE 4096


//ESTRUCTURA TENTATIVA SCHEDULER
typedef struct Scheduler {
	PCB processes[MAX_PROCESSES];
    uint64_t currentPid;
	uint64_t nextPid;
	uint64_t processCount;
	int16_t availableIndex;
	int16_t currIndex;
	int8_t quantumRemaining;
    //uint64_t foregroundPid;       // Foreground process PID
    //uint8_t killForeground;       // Flag to kill foreground process
} Scheduler;

typedef struct ProcessInfo {
	uint64_t pid;
	char name[MAX_LENGTH];
	uint8_t priority;
	char foreground;
	pStatus status;
	uint64_t* stackBase;
	uint64_t pPid;
} ProcessInfo;

typedef struct Scheduler* Sched;

Sched initScheduler();
Sched getScheduler();
int64_t createProcess(char* name, uint8_t priority, char foreground, ProcessEntry func, char** argv, int argc, int* fds);
Process getProcess(uint64_t pid);
uint8_t setStatus(uint8_t newStatus);
uint16_t blockProcess(uint64_t pid);
uint16_t unblockProcess(uint64_t pid);
void* scheduler(void* stackPtr);
Process updateQuantum(void* stackPtr);
uint64_t killProcess(uint64_t pid);
uint64_t changePriority(uint64_t pid, uint8_t newPriority);
ProcessInfo* showProcessesStatus();
void getFDs(int* fds);
void killForegroundProcess();

#endif // SCHEDULER_H
