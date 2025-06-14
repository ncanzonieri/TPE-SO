#ifndef TPE_ARQ_SYSCALLS_H
#define TPE_ARQ_SYSCALLS_H

#include <stdint.h>
#define MAX_LENGTH 32
//#include "library.h"
typedef int (*ProcessEntry)(uint64_t argc, char **argv);
enum State { READY = 0, 
    RUNNING, 
    BLOCKED, 
    TERMINATED, };
typedef struct ProcessInfo {
	uint64_t pid;
	char name[MAX_LENGTH];
	uint8_t priority;
	char foreground;
	enum State status;
    uint64_t* stackPtr;
	uint64_t* stackBase;
	uint64_t pPid;
} ProcessInfo;


/*
 * Arguments used to read the RTC.
 */
#define SECONDS_ARG 0x00
#define MINUTES_ARG 0x02
#define HOURS_ARG 0x04
#define DAY_ARG 0x07
#define MONTH_ARG 0x08
#define YEAR_ARG 0x09

/*
 * File descriptors.
 */
#define STDIN_FD 0
#define STDOUT_FD 1

/*
 * getRegisters data
 */
//#define REGS_AMOUNT 17

enum registers_idx {
    RAX_IDX = 0,
    RBX_IDX,
    RCX_IDX,
    RDX_IDX,
    RSI_IDX,
    RDI_IDX,
    RBP_IDX,
    R8_IDX,
    R9_IDX,
    R10_IDX,
    R11_IDX,
    R12_IDX,
    R13_IDX,
    R14_IDX,
    R15_IDX,
    RIP_IDX,
    RSP_IDX
};


//uint64_t sys_read(int fd, char * buffer, int count);
uint64_t sys_read(uint8_t fd, uint8_t* buffer, uint64_t count);


//uint64_t sys_write(int fd, const char * buffer, int count, uint32_t color);
uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count, uint32_t color);



uint64_t sys_drawRectangle(uint32_t hexColor, uint64_t x, uint64_t y, uint64_t width, uint64_t height);

uint64_t sys_clearScreen();

uint64_t sys_getCoords();

uint64_t sys_getScreenInfo();

uint64_t sys_getFontInfo();

uint64_t sys_getTime(uint64_t arg);

uint64_t sys_setFontScale(uint64_t scale);

uint64_t sys_getRegisters(uint64_t * regs);

uint64_t sys_sleep(uint64_t millis);

uint64_t sys_ticks();

uint64_t sys_playSound(uint64_t f, uint64_t millis);

uint64_t sys_malloc(uint64_t size);

uint64_t sys_free(uint64_t ptr);

uint64_t sys_memoryDump();

uint64_t sys_getPid();
uint64_t sys_killProcess(uint64_t pid);
uint64_t sys_showProcesses();
int64_t sys_createProcess(char* name, uint8_t priority, char foreground, ProcessEntry func, char** argv, int argc, int * fds);
uint64_t sys_changePriority(uint64_t pid, uint8_t priority);
uint64_t sys_blockProcess(uint64_t pid);
uint64_t sys_unblockProcess(uint64_t pid);
int64_t sys_waitForChildren(uint64_t pid);
uint64_t sys_yield();

int64_t sys_semOpen(char* semId, uint64_t initialValue);
int64_t sys_semClose(char* semId);
int64_t sys_semWait(char* semId);
int64_t sys_semPost(char* semId);

int64_t sys_createPipe(int fds[2]);
//
int64_t sys_destroyPipe(int writeFd);

#endif