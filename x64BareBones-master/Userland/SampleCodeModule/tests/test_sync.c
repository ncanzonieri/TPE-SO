#include <stdint.h>
#include <stdio.h>
#include "../include/syscalls.h"
#include "../include/test_utils.h"

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  sys_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (sys_semOpen(SEM_ID, 1) == -1) {
      printf("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sys_semWait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      sys_semPost(SEM_ID);
  }

  if (use_sem)
    sys_semClose(SEM_ID);

  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = sys_createProcess("my_process_inc", 1, 0,&my_process_inc, argvDec, 3);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_createProcess("my_process_inc", 1, 0, &my_process_inc, argvInc, 3);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    sys_semWait(pids[i]);
    sys_semWait(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  printf("Final value: %d\n", global);

  return 0;
}