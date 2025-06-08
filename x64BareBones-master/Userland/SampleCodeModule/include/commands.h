
#ifndef COMMANDS_H  // Prevenir la inclusión múltiple
#define COMMANDS_H
#define MAX_ARGS 3
typedef int (*mainFunction_t)(int argc, char* argv[]);

typedef struct inputCommand {
    char* name;
    char* args[MAX_ARGS+1];
    int argCount;
    int pid;
    int fds[2];
} inputCommand_t;

typedef struct command {
    char* name;
    char* description;
    char processOrCommand;
    mainFunction_t function;
} command_t;

typedef enum {
    DIVX0 = 0,
    INVALID,
    HELP,
    TIME,
    ZOOM,
    REGISTERS,
    AGRO,
    DATE,
    SNAKE,
    CLEAR,
    TEST_MM,
    MEM_DUMP,
    PS,
    TEST_PROC,
    TEST_PRIO,
    TEST_SYNC,
    LOOP,
    KILL,
    NICE,
    BLOCK,
    UNBLOCK,
    INVALID_OPERATION
} commandId_t;

extern command_t commands[INVALID_OPERATION];

/*
command_t commands[INVALID_OPERATION] = {
    {"divx0", "Simula la excepión de dividir por 0.", 0, divx0},
    {"invalid", "Simula la excepción de código de operación inválida.", 0, invalid},
    {"help", "Imprime la lista de los comandos disponibles y su descripción.", 0, help},
    {"time", "Imprime hora actual en Buenos Aires.", 0, actualTime},
    {"zoom", "Varía la escala del texto: <in> la aumenta, <out> la decrementa", 0, zoom},
    {"registers", "Imprime los últimos registros cargados (se cargan con Ctrl+R).", 0, registers},
    {"agro", "Imprime el escudo de Club Atlético Agropecuario.", 0, agro},
    {"date", "Imprime fecha actual en Buenos Aires.", 0, actualDate},
    {"snake", "Comienza el juego de snake." , 0, snake},
    {"clear", "Limpia la pantalla.", 0, sys_clearScreen},
    {"testMemM", "Test del memory manager.", 1, testMM},
    {"mem", "Imprime el uso de memoria actual.", 1, memoryDump},
    {"ps", "Imprime información sobre los procesos vivos al momento.", 1, ps},
    {"testProc", "Test del scheduler.", 1, testProcesses},
    {"testPrio", "Test de prioridades.", 1, testPriorities},
    {"testSync", "Test de sincronización.", 1, testSync},
    {"loop", "Imprime su ID con un saludo cada una determinada cantidad de segundos.", 1, loop},
    {"kill", "Mata un proceso por su PID.", 1, kill},
    {"nice", "Cambia la prioridad de un proceso dado su PID y nueva prioridad", 1, nice},
    {"block", "Bloquea un proceso por su PID.", 1, block},
    {"unblock", "Desbloquea un proceso por su PID.", 1, unblock}
    };
*/
// Declaración de funciones
int help(int argc, char *argv[]);          // Muestra la ayuda sobre los comandos disponibles
int snake(int argc, char *argv[]);         // Ejecuta el juego de la serpiente
int registers(int argc, char *argv[]);     // Muestra el estado de los registros
int invalid(int argc, char *argv[]);  // Maneja el caso de operación inválida
int divx0(int argc, char *argv[]);        // Maneja la excepción de división por cero
int zoom(int argc, char *argv[]);       // Aumenta o disminuye la escala del texto
int actualTime(int argc, char *argv[]);         // Muestra el tiempo actual
int actualDate(int argc, char *argv[]);         // Muestra la fecha de hoy
extern void invalidOperation(); // función de assembler que maneja una operación inválida
int agro(int argc, char *argv[]); // imprime escudo de un equipo de futbol
int testMM(int argc, char *argv[]); // testea el memory manager
int memoryDump(int argc, char *argv[]); // imprime el estado de la memoria
int ps(int argc, char *argv[]); // imprime los procesos en ejecución
int testProcesses(int argc, char *argv[]); // Testea la creación de procesos
int testPriorities(int argc, char *argv[]); // Testea cambio de prioridades
int testSync(int argc, char *argv[]); // Testea la sincronización entre procesos
int loop(int argc, char *argv[]); //  Imprime su ID con un saludo cada una determinada cantidad de segundos.
int kill(int argc, char *argv[]); // Mata un proceso dado su PID
int nice(int argc, char *argv[]); // Cambia la prioridad de un proceso dado su PID y nueva prioridad
int block(int argc, char *argv[]); // Bloquea un proceso dado su PID
int unblock(int argc, char *argv[]); // Desbloquea un proceso dado su PID
#endif // COMMANDS_H
