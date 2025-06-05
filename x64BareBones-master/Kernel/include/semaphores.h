#ifndef SEMAPHORE_MANAGER_H
#define SEMAPHORE_MANAGER_H

#include <stdint.h>
#include "MemoryManagerADT.h"
#include "lib.h"
#include "linkedList.h"
#include "process.h"

#define MAX_SEMAPHORES 20

typedef struct SemManagerCDT * SemManager;

typedef struct SemCDT * Sem;

SemManager createSemManager();
SemManager getSemManager();

int addSem(Sem sem);
Sem getSem(char* semId);


#endif