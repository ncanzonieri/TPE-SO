#include <keyboard2.h>
#include <stdint.h>
#include <videodriver.h>

#define STDIN 1  
#define STDOUT 0  
enum syscallsList { READ=0, WRITE, DRAW_RECTANGLE, GET_COORDS, CLEAR_SCREEN,
 GET_SCREEN_INFO, GET_FONT_INFO, GET_TIME, SET_FONT_SCALE, GET_REGISTERS, SLEEP,
 PLAY_SOUND, SET_BGCOLOR, GET_BGCOLOR};

uint64_t syscallDispatcher(uint64_t rax, uint64_t * otherRegs){
    switch(rax){
        case READ:
            return sys_read((uint8_t) otherRegs[0], (uint8_t *) otherRegs[1], otherRegs[2]);
        case WRITE:
            return sys_write((uint8_t) otherRegs[0], (char *) otherRegs[1], otherRegs[2], (uint32_t) otherRegs[3]);
        default:
            return 0;
    }
}

//USO BUFFER DE KEYBOARD2.C SE LO PASO COMO PARAMETRO
uint64_t sys_read(uint8_t fd, uint8_t* buffer, uint64_t count){
    if(fd != STDIN)
        return 0;

    for(int i = 0; i < count;i++){
        char c = kb_getchar();
        if(c == 0){
            return i; //cuantos caracteres fueron leidos
        }
        //sino cargo en mi buffer 
        buffer[i] = c;
    }
    return count;
}

uint64_t sys_write(uint8_t fd, const char * buffer, uint64_t count, uint32_t color) {
    // STDOUT is the only file descriptor supported so far
    if (fd == STDOUT) {
        return printString(color, buffer);
    }
    return 0;
}

uint64_t sys_drawRectangle(uint32_t hexColor, uint64_t x, uint64_t y, uint64_t width, uint64_t height) {
    return drawRectangle(hexColor, x, y, width, height);
}

uint64_t sys_getCoords() {
    // Return the y in the high 32 bits and the x in the low 32 bits
    
    return getCoords();
}

uint64_t sys_clearScreen() {
    clearScreen();
    return 0;
}

uint64_t sys_getScreenInfo() {
    // return the width in the high 32 bits and the height in the low 32 bits
    return ((uint64_t) getScreenWidth() << 32) | getScreenHeight();
}

uint64_t sys_getFontInfo() {
    // return the width in the high 32 bits and the height in the low 32 bits
    return ((uint64_t) (getFontWidth() * getScale()) << 32) | (getFontHeight() * getScale());
}

uint64_t sys_getTime(uint64_t arg) {
    return getTime(arg);
}

uint64_t sys_setFontScale(uint64_t scale) {
    return (uint64_t) setScale((uint8_t) scale);
}

uint64_t sys_getRegisters(uint64_t * r) {
    return getRegisters(r);
}

uint64_t sys_sleep(uint64_t millis) {
    unsigned long long initial_time = ms_elapsed();
    unsigned long long currentTime = initial_time;
    // Activate interrupts
    _sti();
    while ((currentTime - initial_time) <= millis) {
        currentTime = ms_elapsed();
    }
    // Deactivate interrupts
    _cli();
    return 1;
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