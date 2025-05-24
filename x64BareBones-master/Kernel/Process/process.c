#include "process.h"

//desde el kernel se inicializa el proceso al sheduler, el scheduler guarda el proceso en su array 
//y acá se inicializa el proceso

static void exitProcess(int ret, unsigned int pid) {
	Process process = getProcess(pid);
	process->retValue = ret;
//	killProcess(pid);
//	yield();
}


//Actúa como un envoltorio (wrapper) que ejecuta la función principal del proceso y asegura que el proceso termine correctamente
void runProcessWrapper(ProcessEntry func, char **argv, uint64_t argc) {
    int newArgc = argCount(argv);
    int ret = func(newArgc, argv);
    exitProcess(ret, getPid());
}

void initProcess(PCB* process, char* name, uint16_t pid, uint16_t ppid, uint8_t priority, char** argv, int argc, ProcessEntry func, const int16_t fds[]){
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
    process->stackPtr = _initialize_stack_frame(&runProcessWrapper, func, stackEnd, (void *) process->argv);
}

uint64_t getPid() {
    Sched scheduler = getScheduler();
    return scheduler->currentPid;
}
