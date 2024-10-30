#include <keyboardDriver.h>
#include <stdint.h>
#include <videoDriver.h>
#include <time.h>
#include <sysCalls.h>

#define REGISTERS_DIM 16
#define STDIN 0  
#define STDOUT 1  
enum syscallsList { READ=0, WRITE, DRAW_RECTANGLE, CLEAR_SCREEN, GET_COORDS,
 GET_SCREEN_INFO, GET_SCALE, GET_TIME, SET_SCALE, GET_REGISTERS, SLEEP,
 PLAY_SOUND, SET_BGCOLOR, GET_BGCOLOR};

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
            return 0; // falta implementar
        case SET_BGCOLOR:
            return sys_setBgColor((uint32_t) otherRegs[0]);
        case GET_BGCOLOR:
            return sys_getBgColor();
        default:
            return 0;
    }
}

/*
uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t rax) {
    // rax contains the syscall id
    switch (rax) {
        case READ: return sys_read(rdi, (char *) rsi, rdx);
        case WRITE: 
        printStringInCoord(0x00FFFFFF,"la-maquina 999999999 $>",3,3);
        return _sys_write(rdi, (char *) rsi, rdx, rcx);
        default: return 0;
    }
}
*/

//USO BUFFER DE KEYBOARD2.C SE LO PASO COMO PARAMETRO
uint64_t sys_read(uint8_t fd, uint8_t* buffer, uint64_t count){
    if(fd != STDIN)
        return 0;

    int i = 0;
    for(; i < count;i++){
        char c = kb_getchar();
        if(c == 0){
            return i; //cuantos caracteres fueron leidos
        }
        //sino cargo en mi buffer 
        buffer[i] = c;
    }
    buffer[i+1] = "0";
    return count;
}

uint64_t _sys_write(uint8_t fd, char * buffer, uint64_t count, uint32_t color) {
    // STDOUT is the only file descriptor supported so far
    if (fd == STDOUT) {
        return printString(color, buffer);
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
    loadRegisters(); 
    uint64_t * aux = getRegisters();
    for( int i=0; i<REGISTERS_DIM; i++){
        r[i]=aux[i];
    }
    return 1;
}

uint64_t sys_sleep(uint64_t millis) {
    return sleep(millis);
}
/*
uint64_t sys_playSound(uint64_t f, uint64_t millis) {
    playSound(f);
    sys_sleep(millis);
    stopSound();
    return 1;
}
*/
uint64_t sys_setBgColor(uint32_t color) {
    setBGColor(color);
    return 1;
}

uint64_t sys_getBgColor() {
    return getBGcolor();
}