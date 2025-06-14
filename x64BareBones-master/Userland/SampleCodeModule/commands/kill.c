// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/syscalls.h"
#include "../include/library.h"

int kill(int argc, char *argv[]) {
	if (argc != 1) {
		printf("Usage: kill [pid]\n");
		return -1;
	}
	int pid = atoi(argv[0]);
	int ret = sys_killProcess(pid);
	if (ret == 0) {
		printf("Error killing process\n");
		return -1;
	}
	return 0;
}
