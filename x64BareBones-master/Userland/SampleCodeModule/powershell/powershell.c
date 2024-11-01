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
#define COMMANDS_COUNT 10
#define GREEN 0x66FF66 // Font Scale
#define WHITE 0xFFFFFF
#define ERROR -1
#define EXIT 1

void welcome();
void getCommands();
static void startShell(char * v);
static int belongs(char * v);
static void runCommands(int index);

////FALTA AGREGAR EL SNAKE ANTES DE ZOOMIN
static void (* runFuncts[])() = {divx0, invalid, help, actualTime, zoomIn, zoomOut, registers, agro, actualDate};

static void putUser(){
  sys_write(STDOUT_FD, "la-maquina $>",14,GREEN);
}

#define DEFAULT_SCALE 1

void welcome(){
    sys_clearScreen();
    sys_setFontScale(DEFAULT_SCALE);
    sys_write(STDOUT_FD,"Today's date is: ",18,0x00ffffff);
    actualDate();
}
//uint64_t sys_read(uint8_t fd, uint8_t* buffer, uint64_t count){
//uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count, uint32_t color)

void getCommands(){
    char buffer[MAX_DIM] = {0};
    // char copy[MAX_DIM] = {0};
    int dim; 
    char* c[2]={0};
    int ans;
    while(1) {
        dim=0;
        buffer[0]=0;
        putUser();
        while( (c[0] = getChar()) != '\n'){
            if(c[0]!=0){
            if( c[0] == DELETE){
                if( dim > 0){
                    dim--;
                    putChar(c[0], WHITE);
                }
            }else if( c[0] != ESC){
                buffer[dim++] = c[0];
                sys_write(STDOUT_FD,c,1,WHITE);
            }
            }
        }
        buffer[dim]=0;
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
        sys_write(STDOUT_FD, "command not found, type 'help'\n",31, WHITE);
        //sys_write(STDOUT_FD,v,75,GREEN);
    }else{
        runFuncts[flag]();
    }
}

static int belongs(char * v){ //FALTA AGREGAR EL SNAKE ANTES DE ZOOMIN
    char * commands[COMMANDS_COUNT] = {"divx0", "invalid", "help", "actualTime", "zoomIn", "zoomOut", "registers", "agro","actualDate"};
    for( int i=0; i < COMMANDS_COUNT; i++){
        if( strcmp(v, commands[i]) == 0){
            return i;
        }
    }
    //sys_write(STDOUT_FD,,,WHITE);
return ERROR;
}
