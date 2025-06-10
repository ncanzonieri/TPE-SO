// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/syscalls.h"
#include "../include/library.h"

int loop(int argc, char **argv) {
	if (argc != 1) {
		printf("Usage: loop [ticks]\n");
		return -1;
	}
	int pid = sys_getPid();
	int ticks = atoi(argv[0]);
	if (ticks < 0) {
		printf("Ticks must be a positive integer\n");
		return -1;
	}
	while (1) {
		printf("Hi %d, ", pid);
		sys_sleep(ticks * 1000);
	}
}
