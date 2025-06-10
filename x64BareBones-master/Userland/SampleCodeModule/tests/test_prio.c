// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stdint.h>
#include <stdio.h>
#include "../include/syscalls.h"
#include "../include/test_utils.h"

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 300000000      // TODO: Change this value to make the wait long enough to see these processes beeing run at least twice

#define TOTAL_PROCESSES 4

#define LOWEST 1  // TODO: Change as required
#define MEDIUM 2  // TODO: Change as required
#define MEDIUMHIGH 3
#define HIGHEST 4 // TODO: Change as required

#define FOREGROUND 1

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, MEDIUMHIGH, HIGHEST};

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {"10000000"};
  uint64_t i;
  //printf("TESTING PRIORITIES, my pid: %d\n", sys_getPid());
  int fds[2] = {0, 1};
  for (i = 0; i < TOTAL_PROCESSES; i++)
  pids[i] = sys_createProcess("loopPrnt",1, FOREGROUND, (ProcessEntry) &endless_loop_print, argv, 1, fds);
  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], prio[i]);

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_blockProcess(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], MEDIUM);

  printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_unblockProcess(pids[i]);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_killProcess(pids[i]);

  printf("TEST COMPLETED\n");
}