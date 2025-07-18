#ifndef TPE_ARQ_SYSCALL_LIB_H
#define TPE_ARQ_SYSCALL_LIB_H

#include <stdint.h>
#include <lib.h>

#define REGS_AMOUNT 20

static volatile uint8_t registersFilled = 0;

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

/**
 * @brief Reads a string from the standard input.
 * @param buffer: the buffer to store the string.
 * @param count: the number of chars to read.
 * @return the number of chars read.
 */
uint64_t sys_read(uint8_t fd, uint8_t* buffer, uint64_t count);

/**
 * @brief Writes a string to the standard output.
 * @param buffer: the string to write.
 * @param count: the number of chars to write.
 * @param color: the color of the string in hexadecimal. Usage: 0x00RRGGBB.
 * @return the number of chars written.
 */
uint64_t _sys_write(uint8_t fd, char * buffer, uint64_t count, uint32_t color);

/**
 * @brief Draws a rectangle in the screen on the given coordinates.
 * @param hexColor: the color of the rectangle in hexadecimal. Usage: 0x00RRGGBB.
 * @param x: the x coordinate of the rectangle.
 * @param y: the y coordinate of the rectangle.
 * @param width: the width of the rectangle.
 * @param height: the height of the rectangle.
 * @return 0 if the rectangle was drawn successfully, 1 if the rectangle was out of bounds.
 */
uint64_t sys_drawRectangle(uint32_t hexColor, uint64_t x, uint64_t y, uint64_t width, uint64_t height);

/**
 * @brief Clears the screen.
 */
uint64_t sys_clearScreen();

/**
 * @brief Returns the current cursor position.
 * @return the current cursor y position in the higher 32 bits and the x position in the lower 32 bits.
 */
uint64_t sys_getCoords();

/**
 * @brief Returns the screen width in the high 32 bits and the screen height in the low 32 bits.
 */
uint64_t sys_getScreenInfo();

/**
 * @brief Returns the font width in the high 32 bits and the font height in the low 32 bits.
 */
uint64_t sys_getScale();

/**
 * @brief Returns the time data of the current time read from the RTC. Note: time is in UTC
 * @param rdi: sec=0, min=2, hour=4, day=7, month=8, year=9. Other registers are not implemented.
 */
uint64_t sys_getTime(uint8_t rdi);

/**
 * @brief Sets the font scale. Supported values are 1, 2 and 3.
 * @param scale the scale to set.
 */
uint64_t sys_setFontScale(uint8_t scale);

/**
 * @brief Fills the registers array with the current values of the registers.
 * Registers must be updated with CTRL + R before calling this function.
 * @param r: the array to fill with the registers. Be sure of having at least REGS_AMOUNT elements.
 * @return 0 if the registers were filled successfully, 1 if the registers were not filled.
 */
uint64_t sys_getRegisters(uint64_t * r);

/**
 * @brief Sleeps the current process for the given amount of milliseconds.
 * @param millis: the amount of milliseconds to sleep.
 */
uint64_t sys_sleep(uint64_t millis);

/**
 * @brief Returns the current ticks of the system.
 * @return the current ticks of the system.
 */
uint64_t sys_ticks();

/**
 * @brief Plays a sound with the given frequency.
 * @param f: frequency of the sound.
 * @param millis: the amount of milliseconds to last.
 */
uint64_t sys_playSound(uint64_t f, uint64_t millis);

/**
 * @brief Sets the background color of the screen.
 * @param color: the color to set in hexadecimal. Usage: 0x00RRGGBB.
 */
uint64_t sys_setBgColor(uint32_t color);

/**
 * @brief Returns the current background color of the screen.
 * @return the current background color in hexadecimal. Usage: 0x00RRGGBB.
 */
uint64_t sys_getBgColor();

// SysCalls de Memory Management

/**
 * @brief Allocates a block of memory of the given size.
 * @param size: the size of the block to allocate.
 * @return the address of the block allocated.
 */
uint64_t sys_malloc(uint64_t size);

/**
 * @brief Frees a block of memory.
 * @param ptr: the address of the block to free.
 */
uint64_t sys_free(uint64_t ptr);

/**
 * @brief Returns a pointer to the structure that contains the current memory information.
 * @return the address of the memory information structure.
 */
uint64_t sys_memoryDump();

// SysCalls de Procesos, Context Switch y Scheduling

/**
 * @brief Returns the process ID of the current process.
 * @return the process ID of the current process.
 */
uint64_t sys_getPid();

/**
 * @brief Forces termination of the given process.
 * @param pid process ID of the process that must be terminated.
 */
uint64_t sys_killProcess(uint64_t pid);

int64_t sys_createProcess(char* name, uint8_t priority, char foreground, ProcessEntry func, char** argv, int argc, int * fds);

uint64_t sys_blockProcess(uint64_t pid);

uint64_t sys_unblockProcess(uint64_t pid);

/**
 * @brief Returns a list of the currently running processes 
 * @return process ID of the process that must be terminated.
 */
uint64_t sys_showProcesses();

uint64_t sys_changePriority(uint64_t pid, uint8_t priority);

uint64_t sys_yield();

int64_t sys_waitForChildren(uint64_t pid);

uint64_t sys_finishProcess(/*argumentos*/);

int64_t sys_semOpen(char* semId, uint64_t initialValue);

int64_t sys_semClose(char* semId);

int64_t sys_semWait(char* semId);

int64_t sys_semPost(char* semId);


int64_t sys_createPipe(int fds[2]);

int64_t sys_destroyPipe(int writeFd);

#endif
