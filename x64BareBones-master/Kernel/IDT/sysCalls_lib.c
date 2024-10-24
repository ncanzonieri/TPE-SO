#include <keyboard2.h>

#define STDIN 1   //creo que es el único q deberíamos tener en cuenta 



uint64_t sys_read(int fd, void * buffer, int count){
    if(fd != STDIN)
        return 0;

    
    for(int i = 0; i < count;i++){
        char c = kb_getchar();
        if(c == 0){
            return i;  //cuantos caracteres fueron leidos
        }
        //sino cargo en mi buffer 
        buffer
    }
}