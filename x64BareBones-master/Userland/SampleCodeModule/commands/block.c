#include "../include/syscalls.h"
#include "../include/library.h"

int block(int argc, char *argv[]) {
	if (argc != 1) {
		printf("Usage: block [pid]\n");
		return -1;
	}
	int pid = atoi(argv[0]);
	int ret = sys_blockProcess(pid);
	if (ret == 0) {
		printf("Error blocking process\n");
		return -1;
	}
	return 0;
}