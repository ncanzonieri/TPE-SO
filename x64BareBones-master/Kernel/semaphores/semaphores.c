#include "../include/semaphores.h"

struct SemCDT {
    int id;
    char* name;
    uint64_t value;
    uint8_t mutex;
    LinkedList waitingProcesses;
};

struct SemManagerCDT {
    Sem semaphores[MAX_SEMAPHORES];
    int size;
};

SemManager createSemManager() {
    SemManager semManager = (SemManager)myMalloc(sizeof(struct SemManagerCDT));
    if (semManager == NULL) return NULL;
    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        semManager->semaphores[i] = NULL;
    }
    semManager->size = 0;
    return semManager;
}

SemManager getSemManager() {
    return (SemManager)myMalloc(sizeof(struct SemManagerCDT));
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
