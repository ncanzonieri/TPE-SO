#include "scheduler.h"

//typedef int (*main_function)(uint64_t argc, char **argv);

static int initialized = 0;

Sched initScheduler() {
	Sched scheduler = (Sched) SCHEDULER_ADDRESS;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        scheduler->processes[i].status = TERMINATED;
    }
    scheduler->currentPid = 1;
    scheduler->nextPid = 1;
    scheduler->processCount = 0;
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
    if (scheduler->processCount >= MAX_PROCESSES) return -1;
    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) return -1;
    int availableIndex = -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (scheduler->processes[i].status == TERMINATED) {
            availableIndex = i;
            break;
        }
    }
    if (availableIndex == -1) return -1;
    scheduler->processCount++;
    PCB* process = &scheduler->processes[availableIndex];
	uint64_t pid, ppid;
	if (scheduler->nextPid == INIT_PID) {
		pid = INIT_PID;
		ppid = pid;
		scheduler->nextPid++;
	} else {
		pid = scheduler->nextPid++;
		ppid = getPid();
	}
    char** newArgv = allocArgs(argv, argc);
    if (!newArgv) {
        scheduler->processCount--;
        return -1;
    }

    init_process(process, name, pid, ppid, priority, newArgv, argc, entry_point, fds);
    process->status = READY;
    return pid;
}

//hay que hacer en el asm, el guardado del contexto para luego recuperarlo y hacer una syscall de este
/*
uint64_t createProcess(char *name, Priority priority, char *argv[], int argc, main_function rip, const int16_t fds[]) {
	SchedulerInfo scheduler = get_scheduler();

	int free_spot = scheduler->index_p;
	if (scheduler->amount_processes == MAX_PROCESS || free_spot == -1) {
		return -1;
	}
	scheduler->amount_processes++;
	PCBT *process = &(scheduler->processes[free_spot]);
	process->state = READY;

	update_index_p(scheduler);
	char **new_argv = alloc_arguments(argv, argc);
	int ppid;
	if (scheduler->next_pid == INIT_PID) {
		process->pid = INIT_PID;
		ppid = process->pid;
		scheduler->next_pid++;
	}
	else {
		process->pid = scheduler->next_pid++;
		ppid = get_pid();
	}

	init_process(process, name, process->pid, ppid, priority, new_argv, argc, rip, fds);

	PCBT *parent = find_process(ppid);
	parent->waiting_pid++;

	return process->pid;
}
*/