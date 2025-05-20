#include "process.h"


//desde el kernel se inicializa el proceso al sheduler, el scheduler guarda el proceso en su array 
//y acá se inicializa el proceso
void processInitializing(processCB * p, char * name, Priority priority, char **argv, int_16t pid, int_16t ppid, int_16t fd[], /* int argc, main_function rip,*/){
    
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
	process->stack_pointer = stackFrameInitialization(&process_function, rip, stackEnd, (void *) process->argv);*/
    //stackFrameInitialization en asm --> INTERRUPTS.asm
    return;
}

/*
*/