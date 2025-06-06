#include "../include/semaphores.h"

struct SemCDT {
    int id;
    char *name;
    uint64_t value;
    uint8_t mutex;
    LinkedList waitingProcesses;
};

struct SemManagerCDT {
    Sem semaphores[MAX_SEMAPHORES];
    int size;
};

SemManager createSemManager() {
    SemManager semManager = (SemManager) SEMAPHORE_ADDRESS;
    if (semManager == NULL) return NULL;
    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        semManager->semaphores[i] = NULL;
    }
    semManager->size = 0;
    return semManager;
}

SemManager getSemManager() {
    return (SemManager) SEMAPHORE_ADDRESS;
}

int addSem(Sem sem) {
    SemManager semManager = getSemManager();
    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        if (semManager->semaphores[i] == NULL) {
            semManager->semaphores[i] = sem;
            semManager->semaphores[i]->waitingProcesses = createLinkedList();
            semManager->size++;
            return i;
        }
    }
    return -1;
}

Sem getSem(char *semId) {
    SemManager semManager = getSemManager();
    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        if (semManager->semaphores[i] != NULL && myStrcmp(semManager->semaphores[i]->name, semId) == 0) {
            return semManager->semaphores[i];
        }
    }
    return NULL;
}


static void deleteSem(Sem sem) {
    SemManager semManager = getSemManager();
    semManager->semaphores[sem->id] = NULL;
    semManager->size--;
    myFree(sem->name);
    freeLinkedList(sem->waitingProcesses);
    myFree(sem);
}

static int createSem(char *semId, uint64_t initialValue) {
    Sem sem = (Sem)myMalloc(sizeof(struct SemCDT));
    if (sem == NULL) {
        return -1;
    }

    sem->mutex = 1;
    sem->name = (char *)myMalloc(myStrlen(semId) + 1);
    if (sem->name == NULL) {
        myFree(sem);
        return -1;
    }
    myStrcpy(sem->name, semId);
    sem->value = initialValue;

    int id = addSem(sem);
    if (id == -1) {
        myFree(sem->name);
        myFree(sem);
        return -1;
    }
    sem->id = id;

    return id;
}

int64_t semOpen(char *semId, uint64_t initialValue) {
    Sem existing = getSem(semId);
    if (existing != NULL) {
        return existing->id;
    }

    int id = createSem(semId, initialValue);
    return id;
}

int64_t semWait(char *semId) {
    Sem sem = getSem(semId);
    if (sem == NULL) {
        return -1;
    }
    acquire(&(sem->mutex));
    while (sem->value <= 0) {
        addLast(sem->waitingProcesses, (void *)getPid());
        blockProcess(getPid());
        release(&(sem->mutex));
        _yield();
        acquire(&(sem->mutex));
    }
    sem->value -= 1;
    release(&(sem->mutex));
    return 0;
}

int64_t semPost(char *semId) {
    SemManager semManager = getSemManager();
    Sem sem = getSem(semId);
    if (sem == NULL) {
        return -1;
    }
    acquire(&(sem->mutex));
    semManager->semaphores[sem->id]->value += 1;
    Node *nextProcess = getFirst(sem->waitingProcesses);
    if (nextProcess != NULL) {
        deleteElement(sem->waitingProcesses, (void *)getPid());
        unblockProcess((uint64_t)nextProcess->data);
    }
    release(&(sem->mutex));
    return 0;
}

int64_t semClose(char *semId) {
    Sem sem = getSem(semId);
    if (sem == NULL) {
        return 2;
    }
    if (!isEmpty(sem->waitingProcesses)) {
        return 1;
    }
    acquire(&(sem->mutex));
    deleteSem(sem);
    return 0;
}