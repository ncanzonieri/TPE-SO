// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/syscalls.h"
#include "../include/library.h"

int nice(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: nice [pid] [priority]\n");
		return -1;
	}
	int pid = atoi(argv[0]);
	int priority = atoi(argv[1]);
	if (priority < 1 || priority > 4) {
		printf("Priority must be between 1 and 4\n");
		return -1;
	}
	int ret = sys_changePriority(pid, priority);
	if (ret == 0) {
		printf("Error changing priority\n");
		return -1;
	}
	return 0;
}