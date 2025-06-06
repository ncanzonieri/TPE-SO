#include <keyboardDriver.h>
#include <stdint.h>
#include <videoDriver.h>
#include <audioDriver.h>
#include <MemoryManagerADT.h>
#include <time.h>
#include <sysCalls.h>
#include "../include/scheduler.h"
#include "../include/semaphores.h"

#define REGISTERS_DIM 16
#define STDIN 0  
#define STDOUT 1  
enum syscallsList { READ=0, WRITE, DRAW_RECTANGLE, CLEAR_SCREEN, GET_COORDS,
 GET_SCREEN_INFO, GET_SCALE, GET_TIME, SET_SCALE, GET_REGISTERS, SLEEP,
 PLAY_SOUND, SET_BGCOLOR, GET_BGCOLOR, TICKS, MALLOC, FREE, DUMP, GET_PID, KILL_PROCESS, SHOW_PROCESSES, CREATE_PROCESS, 
 CHANGE_PRIORITY, BLOCK_PROCESS, UNBLOCK_PROCESS, YIELD, SEM_OPEN, SEM_CLOSE, SEM_WAIT, SEM_POST };

extern void loadRegisters();
extern uint64_t* getRegisters();

uint64_t syscallDispatcher(uint64_t rax, uint64_t * otherRegs){
    switch(rax){
        case READ:
            return sys_read((uint8_t) otherRegs[0], (uint8_t *) otherRegs[1], otherRegs[2]);
        case WRITE:
            //sys_write(STDOUT_FD, "la-maquina $>",14,GREEN);
            // printStringInCoord(0x00FFFFFF,"la-maquina 999999999 $>",3,3);
            return _sys_write((uint8_t) otherRegs[0], (char *) otherRegs[1], otherRegs[2], (uint32_t) otherRegs[3]);
        case DRAW_RECTANGLE:
            return sys_drawRectangle((uint32_t)otherRegs[0],otherRegs[1],otherRegs[2],otherRegs[3],otherRegs[4]);
        case GET_COORDS:
            return sys_getCoords();
        case CLEAR_SCREEN:
            return sys_clearScreen();
        case GET_SCREEN_INFO:
            return sys_getScreenInfo();
        case GET_SCALE:
            return sys_getScale();
        case GET_TIME:
            return sys_getTime((uint8_t)otherRegs[0]); 
        case SET_SCALE:
            return sys_setFontScale((uint8_t)otherRegs[0]);
        case GET_REGISTERS:
            return sys_getRegisters((uint64_t*)otherRegs[0]);
        case SLEEP:
            return sys_sleep(otherRegs[0]);
        case PLAY_SOUND:
            return sys_playSound(otherRegs[0], otherRegs[1]);
        case SET_BGCOLOR:
            return sys_setBgColor((uint32_t) otherRegs[0]);
        case GET_BGCOLOR:
            return sys_getBgColor();
        case TICKS: 
            return sys_ticks();
        case MALLOC:
            return sys_malloc(otherRegs[0]);
        case FREE:
            return sys_free(otherRegs[0]);
        case DUMP:
            return sys_memoryDump();
        case GET_PID:
            return sys_getPid();
        case KILL_PROCESS:
            return sys_killProcess(otherRegs[0]);
        case SHOW_PROCESSES:
            return sys_showProcesses();
        case CREATE_PROCESS:
            return sys_createProcess((char*)otherRegs[0], (uint8_t)otherRegs[1], (char)otherRegs[2], (ProcessEntry)otherRegs[3], (char**)otherRegs[4], (int)otherRegs[5]);
        case CHANGE_PRIORITY:
            return sys_changePriority((uint64_t)otherRegs[0], (uint8_t)otherRegs[1]);
        case BLOCK_PROCESS:
            return sys_blockProcess((uint64_t)otherRegs[0]);
        case UNBLOCK_PROCESS:
            return sys_unblockProcess((uint64_t)otherRegs[0]);
        case YIELD:
            return sys_yield(); 
        case SEM_OPEN:
            return sys_semOpen((char*)otherRegs[0], otherRegs[1]);
        case SEM_CLOSE:
            return sys_semClose((char*)otherRegs[0]);
        case SEM_WAIT:
            return sys_semWait((char*)otherRegs[0]);
        case SEM_POST:
            return sys_semPost((char*)otherRegs[0]);
        default:
            return 0;
    }
}

//USO BUFFER DE KEYBOARD2.C SE LO PASO COMO PARAMETRO
uint64_t sys_read(uint8_t fd, uint8_t* buffer, uint64_t count){
    if(fd != STDIN)
        return 0;

    int i = 0;
    for(; i < count;i++){
        char c = getKeyboardChar();
        if(c == 0){
            return i; //cuantos caracteres fueron leidos
        }
        //sino cargo en mi buffer 
        buffer[i] = c;
    }
    return count;
}

uint64_t _sys_write(uint8_t fd, char * buffer, uint64_t count, uint32_t color) {
    // STDOUT is the only file descriptor supported so far
    if (fd == STDOUT) {
        return printStringLength(color, buffer,count);
    }
    return 0;
}

uint64_t sys_drawRectangle(uint32_t hexColor, uint64_t x, uint64_t y, uint64_t width, uint64_t height) {
    drawRectangle(hexColor, x, y, width, height);
    return 1;
}

uint64_t sys_getCoords() {
    // Return the y in the high 32 bits and the x in the low 32 bits
    
    return getCoords();
}

uint64_t sys_clearScreen() {
    // limpia la pantalla
    clearScreen();
    return 0;
}

uint64_t sys_getScreenInfo() {
    // return the width in the high 32 bits and the height in the low 32 bits
    return getScreenSize();
}

uint64_t sys_getScale() {
    return getScale();
}

uint64_t sys_getTime(uint8_t timeOption) {
    return (uint64_t) rtc(timeOption);
}

uint64_t sys_setFontScale(uint8_t scale) {
    return setScale(scale);
}

uint64_t sys_getRegisters(uint64_t * r) {
    //loadRegisters(); 
    return putRegisters(r);
}

uint64_t sys_sleep(uint64_t millis) {
    return sleep(millis);
}

uint64_t sys_playSound(uint64_t f, uint64_t millis) {
    playSound(f);
    sys_sleep(millis);
    stopSound();
    return 1;
}

uint64_t sys_setBgColor(uint32_t color) {
    setBGColor(color);
    return 1;
}

uint64_t sys_getBgColor() {
    return getBGcolor();
}

uint64_t sys_ticks() {
    return ticks_elapsed();
}

uint64_t sys_malloc(uint64_t size) {
    return (uint64_t) myMalloc(size);
}

uint64_t sys_free(uint64_t ptr) {
    myFree((void*)ptr);
    return 1;
}

uint64_t sys_memoryDump() {
    memoryStats_t * stats = memDump();
    return (uint64_t) stats;
}

uint64_t sys_getPid() {
    return getPid();
}

uint64_t sys_killProcess(uint64_t pid) {
    return killProcess(pid);
}

uint64_t sys_showProcesses() {
    return (uint64_t) showProcessesStatus();
}

int64_t sys_createProcess(char* name, uint8_t priority, char foreground, ProcessEntry func, char** argv, int argc){
    int64_t pid = createProcess(name, priority, foreground, func, argv, argc);
    return pid;
}

uint64_t sys_changePriority(uint64_t pid, uint8_t priority){
    return changePriority(pid, priority);
}

uint64_t sys_blockProcess(uint64_t pid){
    return blockProcess(pid);
}

uint64_t sys_unblockProcess(uint64_t pid){
    return unblockProcess(pid);
}

uint64_t sys_yield() {
    _yield();
    return 1;
}

int64_t sys_semOpen(char *semId, uint64_t initialValue) {
    return semOpen(semId, initialValue);
}

int64_t sys_semClose(char *semId) {
    return semClose(semId);
}

int64_t sys_semWait(char *semId) {
    return semWait(semId);
}

int64_t sys_semPost(char *semId) {
    return semPost(semId);
}