// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/syscalls.h"
#include "../include/library.h"

int unblock(int argc, char *argv[]) {
	if (argc != 1) {
		printf("Usage: unblock [pid]\n");
		return -1;
	}
	int pid = atoi(argv[0]);
	int ret = sys_unblockProcess(pid);
	if (ret == 0) {
		printf("Error unblocking process\n");
		return -1;
	}
	return 0;
}