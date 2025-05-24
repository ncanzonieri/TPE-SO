#ifndef PROCESS_H
#define PROCESS_H
#include "../Scheduler/scheduler.h"
#include <stdint.h>
#include <stddef.h>
#include "../include/lib.h"
#include "../include/MemoryManagerADT.h"
#include "../include/interrupts.h"

//Representa el punto de entrada de un proceso, similar a la función main en un programa C.
typedef int (*ProcessEntry)(uint64_t argc, char **argv); //ESTO TAMBIEN SE USA EN KERNEL.C, VER BIEN DONDE DEFINIRLO

#define MAX_LENGTH 32
#define MIN_PRIORITY 1
#define MAX_PRIORITY 4

typedef enum {
    READY = 0, 
    RUNNING, 
    BLOCKED, 
    TERMINATED,
} pStatus;

/*
typedef enum {
    PRIORITY_1 = 1,
    PRIORITY_2,
    PRIORITY_3,
    PRIORITY_4,
}Priority;
*/

typedef struct processControlBlock{
    int16_t pid, pPid, wPid;
    //Priority priority;
    uint8_t priority;
    char name[MAX_LENGTH];
    pStatus status;
    void * stackPtr;
    void * stackBase;
    void * instructionPointer;
//    struct process * children;
    //char foreground;
    //int _16t fd[2];
    char** argv; // Arguments
    uint64_t argc; // Argument count
    int16_t fds[3]; // (STDIN, STDOUT, STDERR)
    int8_t retValue; // Return value
    //    void (*entry_point)(int, char**); // Function to execute
} PCB;

typedef struct processControlBlock* Process;

void initProcess(PCB* process, char* name, uint16_t pid, uint16_t ppid, uint8_t priority, char** argv, int argc, ProcessEntry func, const int16_t fds[]){
uint64_t getPid();
uint64_t getPpid();
//void processInitializing(char * name, Priority priority, char **argv, int16_t pid, int16_t ppid, int16_t fd[]);
//void destroyProcess(process * proc);

#endif
