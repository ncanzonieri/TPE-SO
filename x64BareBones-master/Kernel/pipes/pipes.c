#include "../include/pipes.h"
#include "../include/semaphores.h"
#define BUILTIN_FDS 3 // STDIN, STDOUT, STDERR
#define NAME_SIZE 32
#define EOF -1

typedef struct Pipe {
    char buffer[PIPE_SIZE];
    int fds[2]; // fds[0] = read, fds[1] = write
    int readPos;
    int writePos;
    int size;
    char writeSem[NAME_SIZE];
    char readSem[NAME_SIZE];
} Pipe;

static uint16_t nextFd = BUILTIN_FDS;
static Pipe* pipes[MAX_PIPES];

static int getIndex(int fd, int pos);
static Pipe* initPipe(int fds[2]);

void createPipeManager() {
    for (int i = 0; i < MAX_PIPES; i++) {
        pipes[i] = NULL;
    }
}

int createPipe(int fds[2]) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] == NULL) {
            Pipe* pipe = initPipe(fds);
            if (pipe == NULL) return -1;
            pipes[i] = pipe;
            return i;
        }
    }
    return -1;
}

static Pipe* initPipe(int fds[2]) {
    Pipe* pipe = (Pipe*)myMalloc(sizeof(Pipe));
    if (pipe == NULL) return NULL;

    pipe->readPos = 0;
    pipe->writePos = 0;
    pipe->size = 0;

    pipe->fds[0] = nextFd++;
    fds[0] = pipe->fds[0];
    pipe->fds[1] = nextFd++;
    fds[1] = pipe->fds[1];

    myItoa(pipe->fds[0], pipe->writeSem);
    myStrcat(pipe->writeSem, "_w");

    myItoa(pipe->fds[1], pipe->readSem);
    myStrcat(pipe->readSem, "_r");

    if (semOpen(pipe->writeSem, PIPE_SIZE) == -1) {
        myFree(pipe);
        return NULL;
    }

    if (semOpen(pipe->readSem, 0) == -1) {
        semClose(pipe->writeSem);
        myFree(pipe);
        return NULL;
    }

    return pipe;
}

int writePipe(int fd, const char *buffer, int bytes) {
    if (fd < BUILTIN_FDS || fd >= nextFd) { return -1;  }
    int index = getIndex(fd, 1);
    if (index == -1) {  return -1;  }
    Pipe* pipe = pipes[index];
    for (int i = 0; i < bytes; i++) {
        semWait(pipe->writeSem);
        pipe->buffer[pipe->writePos] = buffer[i];
        pipe->writePos = (pipe->writePos + 1) % PIPE_SIZE;
        pipe->size++;
        semPost(pipe->readSem);
    }
    return bytes;
}

int readPipe(int fd, char *buffer, int bytes) {
    if (fd < BUILTIN_FDS || fd >= nextFd) { return -1;  }
    int index = getIndex(fd, 0);
    if (index == -1) {  return -1;  }
    Pipe* pipe = pipes[index];
    
    for (int i = 0; i < bytes; i++) {
        semWait(pipe->readSem);
        buffer[i] = pipe->buffer[pipe->readPos];
        pipe->readPos = (pipe->readPos + 1) % PIPE_SIZE;
        pipe->size--;
        semPost(pipe->writeSem);
    }
    return bytes;
}

void sendEOF(int fd) {
    if (fd < BUILTIN_FDS || fd >= nextFd) { return; }
    int index = getIndex(fd, 1);
    if (index == -1) {  return; }
    Pipe *pipe = pipes[index];
    semWait(pipe->writeSem);
    pipe->buffer[pipe->writePos] = EOF;
    pipe->writePos = (pipe->writePos + 1) % PIPE_SIZE;
    pipe->size++;
    semPost(pipe->readSem);
}

void destroyPipe(int writeFd) {
    int index = getIndex(writeFd, 1);
    if (index == -1) {  return; }
    Pipe* pipe = pipes[index];
    semClose(pipe->writeSem);
    semClose(pipe->readSem);
    myFree(pipe);
    pipes[index] = NULL;
}

static int getIndex(int fd, int pos) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] != NULL && pipes[i]->fds[pos] == fd) {
            return i;
        }
    }
    return -1;
}
