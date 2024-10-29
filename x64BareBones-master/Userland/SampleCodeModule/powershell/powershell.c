#include <time.h>
#include <commands.h>

#include <syscalls.h>
#include <library.h>

#define MAX 15
#define MAX_DIM 256
#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
#define DELETE 8
#define CERO 0
#define ESC 27
#define TAB 9
#define COMMANDS_COUNT 9
#define GREEN 0x66FF66 // Font Scale
#define WHITE 0xffffff
#define ERROR -1
#define EXIT 1

void welcome();
void getCommands();
static void startShell(char * v);
static int belongs(char * v);
static void runCommands(int index);

static void (* runFuncts[])() = {divx0, invalid, help, actualTime, snake, zoomIn, zoomOut, registers};

static void putUser(){
  sys_write(STDOUT_FD, "la-maquina $>",14,GREEN);
}

#define DEFAULT_SCALE 1

void welcome(){
    sys_clearScreen();
    sys_setFontScale(DEFAULT_SCALE);
    actualTime();
}
//uint64_t sys_read(uint8_t fd, uint8_t* buffer, uint64_t count){
//uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count, uint32_t color)

void getCommands(){
    char buffer[MAX_DIM] = {0};
    // char copy[MAX_DIM] = {0};
    int dim = 0; 
    char c;
    int ans;
    while(1) {
        putUser();
        while( (c = getChar()) != '\n'){
            if( c == TAB){
                for( int i=0; i< 4; i++){
                    putChar(c,GREEN);
                    dim++;
                    if( dim < MAX_DIM){
                        buffer[dim++] = ' ';
                    }
                }
            }else if( c == DELETE){
                if( dim > 0){
                    dim--;
                    putChar(c, WHITE);
                }
            }else if( c != ESC){
                buffer[dim++] = c;
                sys_write(STDOUT_FD,&c,1,GREEN);
            }
        }
        putChar('\n',WHITE);
        startShell(buffer);
    }
    
}





static void startShell(char * v){
    if( *v == 0){ 
        return;
    }
    int flag = belongs(v);
    if( flag == ERROR ){
        sys_write(STDOUT_FD, "command not found, type 'help'",31, GREEN);
    }
    runCommands(flag);
}


static int belongs(char * v){
    char * commands[COMMANDS_COUNT] = {"divx0", "invalid", "help", "actualTime", "snake", "zoomIn", "zoomOut", "registers"};
    for( int i=0; i < COMMANDS_COUNT; i++){
        if( strcmp(v, commands[i]) == 0){
            return i;
        }
    }
    //sys_write(STDOUT_FD,,,WHITE);
return ERROR;
}

static void runCommands(int index){
    if( index == ERROR){
        help();
// call_sysError(); // dps cambiamos las funciones, osea printf("Error not found")
    }
    runFuncts[index](); 

}
