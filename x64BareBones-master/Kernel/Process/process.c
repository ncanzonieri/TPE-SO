#include "process.h"

//desde el kernel se inicializa el proceso al sheduler, el scheduler guarda el proceso en su array 
//y acá se inicializa el proceso
/*
//void processInitializing(processCB * p, char * name, Priority priority, char **argv, int16_t pid, int16_t ppid, int16_t fd[], /* int argc, main_function rip,){
    
    // Initialize process control block
    //strncpy(process->name, name, sizeof(process->name)); crear strncpy
    p->priority = priority;
    p->argv = argv;
    p->pid = pid;
    p->ppid = ppid;
    //p->fd = fd; debería haber una asignación de fd
    
    /*
    process->stack_base = alloc_memory(STACK_SIZE);
	if (process->stack_base == NULL) {
		draw_with_color("Error: Could not allocate memory for process", 0xFF0000);
		free_memory(process->stack_base);
		return;
	}
	void *stackEnd = (void *) ((uint64_t) process->stack_base + STACK_SIZE);
	process->argc = argc;
	//process->stack_pointer = stackFrameInitialization(&process_function, rip, stackEnd, (void *) process->argv);
    //stackFrameInitialization en asm --> INTERRUPTS.asm
    return;
}
*/


static void exitProcess(int ret, unsigned int pid) {
	Process process = getProcess(pid);
	process->retValue = ret;
//	killProcess(pid);
//	yield();
}

void pFunc(function func, char **argv, uint64_t argc) {
    int newArgc = argCount(argv);
    int ret = func(newArgc, argv);
    exitProcess(ret, getPid());
}

void initProcess(PCB* process, char* name, uint16_t pid, uint16_t ppid, uint8_t priority, char** argv, int argc, function func, const int16_t fds[]){
    strncpy(process->name, name, sizeof(process->name));
    process->pid = pid;
	process->pPid = ppid;
	//process->wPid = ;
	process->priority = priority;
	//process->times_to_run = priority;
    // asignFD(); x3
    process->argc = argc;
    process->argv = argv;
    process->stackBase = myMalloc(STACK_SIZE);
    if (process->stackBase == NULL) {
        //error
        myFree(process->stackBase);
        return;
    }
    void *stackEnd = (void *) ((uint64_t) process->stackBase + STACK_SIZE);
    process->stackPtr = _initialize_stack_frame(&pFunc, func, stackEnd, (void *) process->argv);
}

uint64_t getPid() {
    Sched scheduler = getScheduler();
    return scheduler->currentPid;
}
