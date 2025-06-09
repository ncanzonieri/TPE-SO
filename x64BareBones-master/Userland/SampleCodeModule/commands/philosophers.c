// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <syscalls.h>
#include <library.h>
#include <commands.h>
#include <philosophers.h>

#define MAX_PHILO 10
#define MIN_PHILO 3

typedef enum states{
    THINKING = 0,
    EATING, 
    HUNGRY
} states;

typedef struct {
    int state;
    int pid;
} Philosopher;

Philosopher philosophers[MAX_PHILO];
int philoCount = 0;
char* mutex = "mutexPhilo";

char* forkNames[MAX_PHILO] = {
    "fork0", "fork1", "fork2", "fork3", "fork4",
    "fork5", "fork6", "fork7", "fork8", "fork9",
};

int phylo(int argc, char **argv) {
    sys_semOpen(mutex, 1);
    printInstructions();

    for (int i = 0; i < MIN_PHILO; i++) {
        addPhilo(i);
    }

    int running = 1;
    while (running) {
        char key = getChar();
        running = processInput(key);
    }

    for(int i = 0; i < philoCount; i++) {
        sys_semClose(forkNames[i]);
        sys_killProcess(philosophers[i].pid);
    }
    philoCount = 0;
    sys_semClose(mutex);
    return 0;
}

void canEat(int i) {
    int left = (i + philoCount - 1) % philoCount;
    int right = (i + 1) % philoCount;
    if(philosophers[i].state == HUNGRY && philosophers[left].state != EATING && philosophers[right].state != EATING){
        philosophers[i].state = EATING;
        sys_semPost(forkNames[i]);
        printStatus();
    }
}

void think() {
    sys_sleep(5);
}

void eat() {
    sys_sleep(10);
}

void takeForks(int i) {
    sys_semWait(mutex);
    philosophers[i].state = HUNGRY;
    canEat(i);
    sys_semPost(mutex);
    sys_semWait(forkNames[i]);
}

void putForks(int i) {
    sys_semWait(mutex);
    philosophers[i].state = THINKING;
    canEat((i + philoCount - 1) % philoCount);
    canEat((i + 1) % philoCount);
    sys_semPost(mutex);
}

int philosopher(int argc, char **argv) {
    int i = atoi(argv[0]);
    philosophers[i].pid = sys_getPid();

    while (1) {
        think();
        
        takeForks(i);
        
        if (philosophers[i].state == EATING) {
            eat();
            putForks(i);   
        }
        printStatus();
    }
    return 0;
}

void addPhilo(int i) {
    
    if (philoCount >= MAX_PHILO) {
        printf("Maximo de filosofos alcanzado, no se puede agregar mas.\n");
        return;
    }
    sys_semWait(mutex);
    philosophers[i].state = THINKING;

    sys_semOpen(forkNames[i], 1);

    char philoNumber[4];
    itoa(i, philoNumber);
    char *argv[] = {philoNumber, 0};

    int fds[2] = {0, 1};
    philosophers[i].pid = sys_createProcess("philosopher", 1, 0, (ProcessEntry)&philosopher, argv, 1, fds);
    if(philosophers[i].pid < 0) {
        printf("Error al crear el filosofo %d\n", i);
        sys_semPost(mutex);
        return;
    }
    philoCount++;
    printf("Filosofo %d creado\n", philoCount);
    printStatus();
    sys_semPost(mutex);
}

void removePhilo(int i) {
    
    if (philoCount <= MIN_PHILO) {
        printf("No se puede eliminar mas filosofos, minimo alcanzado.\n");
        return;
    }
    sys_semWait(mutex);
    int right = (i + 1) % philoCount;
    int left = (i + philoCount - 1) % philoCount;
    while(philosophers[left].state == EATING && philosophers[right].state == EATING) {
        sys_semPost(mutex);
        sys_semWait(forkNames[i]);
        sys_semWait(mutex);
    }
    sys_semClose(forkNames[i]);
    sys_killProcess(philosophers[i].pid);
    printf("Rip filosofo %d\n", philoCount);
    philoCount--;
    printStatus();
    sys_semPost(mutex);
}

void printInstructions() {
    printf("\nWelcome to Philosophers!\n");
    printf("\nMinimum philosophers: %d, Maximum philosophers: %d\n", MIN_PHILO, MAX_PHILO);

    printf("Press 'a' to add a philosopher\n");
    printf("Press 'r' to remove a philosopher\n");
    printf("Press 'e' to exit\n");
    printf("Press 'c' to clear the screen\n");
    printf("Press 's' to start\n");
    char c = 0;
    while (c != 's') {
        c = getChar();
    }
    printf("\nPhylo starts in 3...");
    sys_sleep(1000);
    printf("2...");
    sys_sleep(1000);
    printf("1...");
    sys_sleep(1000);
    sys_clearScreen();
}

int processInput(char c) {
    if (c == 'a') {
        addPhilo(philoCount);
        return 1;
    }
    if (c == 'r') {
        removePhilo(philoCount - 1);
        return 1;
    }
    if(c == 'c'){
        sys_clearScreen();
        printStatus();
        return 1;
    }
    if (c == 'e') {
        return 0;
    }
    return 1;
}

void printStatus() {
    clearIfNotEnoughSpace(1);
    printf("\t\t\t\t");
    for (int i = 0; i < philoCount; i++) {
        if (philosophers[i].state == EATING)
            putChar('E', 0x00FFFFFF);
        else
            putChar('.', 0x00FFFFFF);
        putChar(' ', 0x00FFFFFF);
    }
    printf("\n");
}
