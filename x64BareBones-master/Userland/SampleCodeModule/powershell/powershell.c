#include <time.h>
#include <commands.h>
#include <string.h>
#include <syscalls.h>

#define MAX 15
#define MAX_DIM 256

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

static void (* runFuncts[])() = {divx0, codOpInvalid, help, time, snake, zoomIn, zoomOut};

////////jijijijijj
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
    
    char input[MAX_DIM] = {0};
    char copy[MAX_DIM] = {0};
    int index = 0; 
    char c;
    int ans;
    while(ans != EXIT) {
        //if(noScreenSpace()) {
        //    parseCommand("clear");
        //}
        putUser();
        getInputAndPrint(input);
        putChar('\n',WHITE);
        strcpy(copy, input);
        startShell(copy);
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
    char * commands[COMMANDS_COUNT] = { "registers", "divx0", "codOpInvalid", "help", "snake", "time", "zoomIn", "zoomOut"};
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
