

#define PROCESS_QUANTITY 30
//ESTRUCTURA TENTATIVA SCHEDULER
typedef struct Scheduler {
	processCB processes[PROCESS_QUANTITY];
	uint16_t processIndex; // va a estar en un lugar libre, si esta lleno valdra -1
	int8_t quantumRemaining;
	uint32_t processCount;
    uint16_t actualPid;
	uint64_t nextPid;
} Scheduler;

uint64_t createProcess(char *name, Priority priority, char *argv[], int argc, main_function rip, const int16_t fds[]);
