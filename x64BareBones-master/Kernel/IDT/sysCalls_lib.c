#include <keyboard2.h>

#define STDIN 1  
#define STDOUT 0  


//USO BUFFER DE KEYBOARD2.C SE LO PASO COMO PARAMETRO
uint64_t sys_read(int fd, void * buffer, int count){
    if(fd != STDIN)
        return 0;

    for(int i = 0; i < count;i++){
        char c = kb_getchar();
        if(c == 0){
            return i;  //cuantos caracteres fueron leidos
        }
        //sino cargo en mi buffer 
        buffer[i] = c;
    }
    return count;  //retorna todos los caracteres leidos
}

uint64_t sys_write(int fd, const char * buffer, int count, uint32_t color) {
    // STDOUT is the only file descriptor supported so far
    if (fd == STDOUT) {
        for(int i = 0; i < count; i++) {
            if(buffer[i] != ESC) {
                // Check if the character fits in the screen
                if ((print_x + getFontWidth() * getScale()) > getScreenWidth()) {
                    print_x = 0;
                    print_y += getFontHeight() * getScale();
                }
                if ((print_y + getFontHeight() * getScale()) > getScreenHeight()) {
                    // No more space in the screen, return the number of characters written
                    return count;
                }
                // Check if the character is a special case
                if (!printSpecialCases(buffer[i])) {
                    drawChar(buffer[i], color, bgColor, print_x, print_y);
                    print_x += getFontWidth() * getScale();
                }
            }
        }
        return count;
    }
    return 0;
}