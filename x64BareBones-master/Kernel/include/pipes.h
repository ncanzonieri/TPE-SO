#ifndef TPESO_PIPES
#define TP2SO_PIPES


#include "../include/lib.h"
#include "../include/MemoryManagerADT.h"
#include "../include/semaphores.h"

#define MAX_PIPES 12
#define PIPE_SIZE 1024 //1 byte de información
#define STDIN 0  
#define STDOUT 1
#define STDERR 2

void createPipeManager();
int createPipe(int fds[2]);
int writePipe(int fd, const char *buffer, int bytes);
int readPipe(int fd, char *buffer, int bytes);
void destroyPipe(int writeFd);
void sendEOF(int fd);

#endif // TP2_SO_PIPES_H