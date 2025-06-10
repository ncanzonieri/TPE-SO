#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <stdint.h>
#include <stddef.h>
#include "./lib.h"
#include "./MemoryManagerADT.h"
#include "./interrupts.h"

#define MAX_LENGTH 32
#define MIN_PRIORITY 1
#define MAX_PRIORITY 4

typedef enum {
    READY = 0, 
    RUNNING, 
    BLOCKED, 
    TERMINATED,
} pStatus;

typedef struct processControlBlock{
    uint64_t pid, pPid, wPid;
    uint8_t priority;
    char name[MAX_LENGTH];
    pStatus status;
    void * stackPtr;
    void * stackBase;
    uint8_t timesToRun;
    char foreground;
    char** argv; // Arguments
    uint64_t argc; // Argument count
    int8_t retValue; // Return value
    int read;
    int write;
} PCB;

typedef struct processControlBlock* Process;

void initProcess(Process process, char* name, uint64_t pid, uint64_t ppid, uint8_t priority, char foreground, char** argv, int argc, ProcessEntry func, int* fds);
uint64_t getPid();
uint64_t getPpid();
int64_t waitChildren(uint64_t pid);

#endif
