#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>

#define MAX_LENGTH 32

typedef enum {
    READY = 0, 
    RUNNING, 
    BLOCKED, 
    TERMINATED,
} processStatus;

typedef enum {
    PRIORITY_1 = 1,
    PRIORITY_2,
    PRIORITY_3,
    PRIORITY_4,
}Priority;

typedef struct processCB{
    int_16t pid, ppid, wpid;
    Priority priority;
    char name[MAX_LENGTH];
    processStatus status;
    void * stackPointer;
    void * basePointer;
    void * instructionPointer;
    struct process * children;
    //char foreground;
    //int _16t fd[2];
} processCB;

void processInitializing(char * name, Priority priority, char **argv, int_16t pid, int_16t ppid, int_16t fd[]);

void destroyProcess(process * proc);

#endif