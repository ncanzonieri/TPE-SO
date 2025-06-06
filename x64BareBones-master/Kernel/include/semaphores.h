#ifndef SEMAPHORE_MANAGER_H
#define SEMAPHORE_MANAGER_H

#include <stdint.h>
#include "MemoryManagerADT.h"
#include "lib.h"
#include "linkedList.h"
#include "process.h"

#define SEMAPHORE_ADDRESS 0x70000
#define MAX_SEMAPHORES 20

typedef struct SemManagerCDT * SemManager;

typedef struct SemCDT * Sem;

SemManager createSemManager();
SemManager getSemManager();

int addSem(Sem sem);
Sem getSem(char *semId);

int64_t semOpen(char *semId, uint64_t initialValue);
int64_t semWait(char *semId);
int64_t semPost(char *semId);
int64_t semClose(char *semId);

#endif 