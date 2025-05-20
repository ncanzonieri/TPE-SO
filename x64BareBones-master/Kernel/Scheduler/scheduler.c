

//typedef int (*main_function)(uint64_t argc, char **argv);

//hay que hacer en el asm, el guardado del contexto para luego recuperarlo y hacer una syscall de este
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