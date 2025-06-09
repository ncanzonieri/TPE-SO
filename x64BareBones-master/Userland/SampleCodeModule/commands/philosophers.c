#include <syscalls.h>
#include <library.h>
#include <commands.h>
#include <philosophers.h>

#define MAX_PHILOSOPHERS 10
#define MIN_PHILOSOPHERS 2

typedef enum states{
    THINKING = 0,
    EATING, 
    HUNGRY
} states;

typedef struct{
    states state;
    int pid;
    int leftFork;
    int rightFork;
} Philosopher;

char* forks[MAX_PHILOSOPHERS] = {
    "fork0", "fork1", "fork2", "fork3", "fork4",
    "fork5", "fork6", "fork7", "fork8", "fork9"
};

char* mutex = "mutexPhilo";

Philosopher philosophers[MAX_PHILOSOPHERS];
int philosophersQuantity;
int changeSem;

int phylo(int argc, char** argv) {
    philosophersQuantity = 0;
    changeSem = sys_semOpen(mutex, 1);
    if (changeSem < 0) {
        printf("Error creating change semaphore\n");
        return -1;
    }
    char* args[MIN_PHILOSOPHERS][2];
    for (int i = 0; i < MAX_PHILOSOPHERS; i++) {
        if(sys_semOpen(forks[i], 1) < 0) {
            printf("Error creating fork semaphore %d\n", i);
            sys_semClose(mutex);
            for (int j = 0; j < i; j++) {
                sys_semClose(forks[j]);
            }
            return -1;
        }
        philosophers[i].state = THINKING;
        
    }

    int* vec;
    vec[0] = 0;
    vec[1] = 1;

    for(int i=0; i<MIN_PHILOSOPHERS; i++){
        itoa(i, args[i][0]);
        args[i][1] = 0;
        philosophers[i].pid = sys_createProcess("philosopher", 1, 0, &philosopher, args[i], 1, vec);
        if (philosophers[i].pid < 0) {
            printf("Error creating philosopher %d\n", i);
            sys_semClose(mutex);
            for (int j = 0; j < MAX_PHILOSOPHERS; j++) {
                sys_semClose(forks[j]);
            }
            return -1;
        }
        philosophers[i].state = THINKING;
        philosophers[i].leftFork = sys_semOpen(forks[i], 1);
        philosophers[i].rightFork = sys_semOpen(forks[(i + 1) % MAX_PHILOSOPHERS], 1);
    }

    printInstructions();
    
    while (1) {
        char c = getChar();
        if (!processInput(c)) {
            break;
        }
        
    }
    
    return 0;
}

int philosopher(int argc, char** argv) {
    int id = atoi(argv[0]);
    char even = id % 2;
    while (1) {
        sys_sleep(1000); // Simulate thinking time
        //sys_semWait(mutex);
        if(even){
            sys_semWait(forks[(id + philosophersQuantity - 1)%philosophersQuantity]);
            sys_semWait(forks[id]);
            philosophers[id].state = EATING;
        }else{
            sys_semWait(forks[id]);
            sys_semWait(forks[(id + philosophersQuantity - 1)%philosophersQuantity]);
            philosophers[id].state = EATING;
        }
        //sys_semPost(mutex);
        sys_sleep(1000); // Simulate eating time
        //sys_semWait(mutex);
        sys_semPost(forks[id]);
        sys_semPost(forks[(id+1)%philosophersQuantity]);
        philosophers[id].state = THINKING;
        //sys_semPost(mutex);
        sys_semWait(mutex);
        printPhilosopherStatus();
        sys_semPost(mutex);
    }
}

int availableToEat(int i){
    int left = (i + philosophersQuantity - 1) % philosophersQuantity;
    int right = (i+1) % philosophersQuantity;
    return philosophers[left].state != EATING && philosophers[right].state != EATING;
}

void thinking(int i){
    sys_sleep(1000);
}

void eating(int i){
    sys_sleep(2000);
}

void takeForks(int i){
    philosophers[i].state = HUNGRY;
    if(availableToEat(i)){
        philosophers[i].state = EATING;
        sys_semWait(philosophers[i].leftFork);
        sys_semWait(philosophers[i].rightFork);
    }
}

void putForks(int i){
    philosophers[i].state = THINKING;
    sys_semPost(philosophers[i].leftFork);
    sys_semPost(philosophers[i].rightFork);
}

void addPhilosopher(){
    sys_semWait(mutex);
    if(philosophersQuantity < MAX_PHILOSOPHERS){
        int i = philosophersQuantity;
//        int idForkLeft = sys_semOpen();
//        int idForkRight = sys_semOpen();
        philosophers[i].state = THINKING;
//        philosophers[i].leftFork = sys_semOpen(idForkLeft, 1);
//        philosophers[i].rightFork = sys_semOpen(idForkRight, 1);
        char philoNumber[4];
        itoa(i, philoNumber);
        char *argv[] = {philoNumber, 0};
        philosophersQuantity++;
        int* vec;
        vec[0] = 0;
        vec[1] = 1;
        sys_createProcess("philosopher", 1, 0, &philosopher, argv, 1, vec);
    }
    sys_semPost(mutex);
}

void removePhilosopher(){
    sys_semWait(mutex); //changeSem
    philosophersQuantity--;
    sys_semClose(philosophers[philosophersQuantity].leftFork);
    sys_semClose(philosophers[philosophersQuantity].rightFork);
    sys_killProcess(philosophers[philosophersQuantity].pid);

    sys_semPost(mutex); //changeSem
}

int processInput(char c) {
    if (c == 'a') {
        addPhilosopher();
        return 1;
    } else if (c == 'r') {
        if (philosophersQuantity > MIN_PHILOSOPHERS)
            removePhilosopher();
        return 1;
    } else if (c == 'e') {
        return 0;
    }
    return 1;
}

void printInstructions() {
    printf("\n\t\t\tWelcome to Philosophers!\n");
    printf("\t\t\tPress 'a' to add a philosopher\n");
    printf("\t\t\tPress 'r' to remove a philosopher\n");
    printf("\t\t\tPress 'e' to exit\n");
}

void printPhilosopherStatus() {
    printf("\t\t\t\t");
    for (int i = 0; i < philosophersQuantity; i++) {
        if (philosophers[i].state == EATING) { 
            putChar('E', 0x00FFFFFF);
        } else {
            putChar('.', 0x00FFFFFF);
        }
        putChar(' ', 0x00FFFFFF);
    }
    printf("\n");
}
