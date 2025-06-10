// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <commands.h>
#include <powershell.h>
#include <syscalls.h>
#include <library.h>

#define MAX 15
#define MAX_DIM 256
#define BLACK 0x00000000
#define DELETE 8
#define CERO 0
#define ESC 27
#define TAB 9
#define COMMANDS_COUNT 25 
#define GREEN 0x66FF66 // Font Scale
#define WHITE 0xFFFFFF
#define ERROR -1
#define EXIT 1

void welcome();
void getCommands();
static void startShell(char * v);
static int parser(char * input, inputCommand_t * command);
int commandArgs(inputCommand_t* cmd, char * input);
//static void runCommands(int index);

command_t commands[INVALID_OPERATION] = {
    {"divx0", "Simula la excepion de dividir por 0.", 0, divx0},
    {"invalid", "Simula la excepcion de codigo de operacion invalida.", 0, invalid},
    {"help", "Imprime la lista de los comandos disponibles y su descripcion.", 0, help},
    {"time", "Imprime hora actual en Buenos Aires.", 0, actualTime},
    {"zoom", "Varia la escala del texto: <in> la aumenta, <out> la decrementa", 0, zoom},
    {"registers", "Imprime los ultimos registros cargados (se cargan con Ctrl+R).", 0, registers},
    {"agro", "Imprime el escudo de Club Atletico Agropecuario.", 0, agro},
    {"date", "Imprime fecha actual en Buenos Aires.", 0, actualDate},
    {"snake", "Comienza el juego de snake." , 0, snake},
    {"clear", "Limpia la pantalla.", 0, (mainFunction_t) sys_clearScreen},
    {"testMem", "Test del memory manager.", 1, testMM},
    {"mem", "Imprime el uso de memoria actual.", 1, memoryDump},
    {"ps", "Imprime informacion sobre los procesos vivos al momento.", 1, ps},
    {"testProc", "Test del scheduler.", 1, testProcesses},
    {"testPrio", "Test de prioridades.", 1, testPriorities},
    {"testSync", "Test de sincronizacion.", 1, testSync},
    {"loop", "Imprime su ID con un saludo cada una determinada cantidad de segundos.", 1, loop},
    {"kill", "Mata un proceso por su PID.", 1, kill},
    {"nice", "Cambia la prioridad de un proceso dado su PID y nueva prioridad", 1, nice},
    {"block", "Bloquea un proceso por su PID.", 1, block},
    {"unblock", "Desbloquea un proceso por su PID.", 1, unblock},
    {"cat", "Imprime el contenido de la entrada estandar.", 1, cat},
    {"wc", "Cuenta las lineas de la entrada estandar.", 1, wc},
    {"filter", "Filtra las vocales de la entrada estandar.", 1, filter},
    {"phylo", "Implementa el problema de los filosofos comensales.", 1, phylo},
};

static void putUser(){
  sys_write(STDOUT_FD, "la-maquina-del-mal$>",21,GREEN);
}

#define DEFAULT_SCALE 1

void welcome(){
    sys_clearScreen();
    sys_setFontScale(DEFAULT_SCALE);
    sys_write(STDOUT_FD,"Hoy es: ",9,0x00ffffff);
    actualDate(0,NULL);
}
//uint64_t sys_read(uint8_t fd, uint8_t* buffer, uint64_t count){
//uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count, uint32_t color)

void getCommands(){
    char buffer[MAX_DIM] = {0};
    int dim; 
    char c=0;
    while(1) {
        dim=0;
        buffer[0]=0;
        putUser();
        while( (c = getChar()) != '\n'){
            if(c!=0){
                if( c == DELETE){
                    if( dim > 0){
                        dim--;
                        putChar(c, WHITE);
                    }
                }else if( c != ESC){
                    buffer[dim++] = c;
                    sys_write(STDOUT_FD,&c,1,WHITE);
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
    inputCommand_t cmd;
    parser(v, &cmd);
}

static int parser(char * input, inputCommand_t * command){
    if(input == NULL){
        return ERROR;
    }

    inputCommand_t inputCommands[2]; //dos comandos máximo
    for(int i = 0; i < 2; i++){
        inputCommands[i].pid = -1;
    }
    int foreground = 1;
    char * pipe = strchr(input, '|');
    size_t commandsToExecute = pipe != NULL ? 2:1;
    if(pipe){
        *pipe = 0;
        if(strchr(pipe+1,'|')){
            printf("Solo está permitido un pipe\n");
            return ERROR;
        }
    }
    for(int i = 0; i < commandsToExecute; i++){
        foreground = foreground && commandArgs(&inputCommands[i],input);
        if(inputCommands[i].argCount == -1){return -1;}
        if(pipe) { input = pipe+1; }
    }
    
    if(pipe){
        int fds[2];
        if(sys_createPipe(fds) == -1){
            printf("Error al crear pipe");
            return ERROR;
        }
        inputCommands[0].fds[1] = fds[1];
        inputCommands[0].fds[0] = STDIN_FD;
        inputCommands[1].fds[0] = fds[0];
        inputCommands[1].fds[1] = STDOUT_FD;
    } else {
        inputCommands[0].fds[0] = foreground ? STDIN_FD : -1;
        inputCommands[0].fds[1] = STDOUT_FD;
    }
    for(int i = 0; i < commandsToExecute;i++){
        uint8_t flag = 0;
        for(int j = 0; j < INVALID_OPERATION;j++){
            if(strcmp(inputCommands[i].name,commands[j].name) == 0){
                flag = 1;
                
                if(!commands[j].processOrCommand){
                    return commands[j].function(inputCommands[i].argCount,inputCommands[i].args);
                }
                else{
                    int* vector = {0};
                    vector[0] = inputCommands[i].fds[0];
                    vector[1] = inputCommands[i].fds[1];
                    inputCommands[i].pid = sys_createProcess(inputCommands[i].name, 1, foreground, (ProcessEntry)commands[j].function, inputCommands[i].args, 
                        inputCommands[i].argCount, vector);
					if (inputCommands[i].pid == -1) {
                        printf("Error creating process.");
						return ERROR;
					}
                }break;
            }
        }
        if(!flag){
            printf( "Command not found.\n");
			for (int j = 0; j < commandsToExecute; j++) {
				if (inputCommands[j].pid != -1) {
					sys_killProcess(inputCommands[j].pid);
				}
			}
			return ERROR;
        }
        
    }
    if (foreground) {
		for (int i = 0; i < commandsToExecute; i++) {
			if (inputCommands[i].pid != -1) {
				sys_waitForChildren(inputCommands[i].pid);
			}
		}
		if (pipe) {
			sys_destroyPipe(inputCommands[0].fds[1]);
		}
	}
	return 0; 
}

int commandArgs(inputCommand_t* cmd, char * input){
    int foreground = 1;
    char* copy = input;
    while(*copy == ' '){ 
        copy++;
    }
    cmd->name = copy;
    int argCount = 0;
    while(*copy != '\0'){
        if(*copy == '&' && (*(copy+1) == ' ' || *(copy+1) == '\0')){
            foreground = 0;
            *copy = '\0';
            break;
        }
        if(*copy == ' '){
            *copy = '\0';
            while(*(copy+1) == ' '){
                copy++;
            }
            if(*(copy+1) != '\0' && *(copy+1) != '&'){
                cmd->args[argCount++] = copy + 1;
                if(argCount >= MAX_ARGS){
                    printf("Máximo de 3 argumentos alcanzado.\n");
                    break;
                }
            }
        }
        copy++;
    }
    cmd->args[argCount] = NULL; // Terminar la lista de argumentos
    cmd->argCount = argCount;
    return foreground;
}

