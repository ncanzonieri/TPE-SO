
#ifndef COMMANDS_H  // Prevenir la inclusión múltiple
#define COMMANDS_H
#define MAX_ARGS 3
typedef int (*mainFunction_t)(int argc, char* argv[]);

typedef struct inputCommand {
    char* name;
    char* args[MAX_ARGS+1];
    int argCount;
    int pid;
} inputCommand_t;

typedef struct command {
    char* name;
    char* description;
    char foreground;
    mainFunction_t function;
} command_t;

typedef enum {
    DIVX0 = 0,
    INVALID,
    HELP,
    TIME,
    ZOOM_IN,
    ZOOM_OUT,
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
} commandId_t;

// Declaración de funciones
void help();          // Muestra la ayuda sobre los comandos disponibles
void snake();         // Ejecuta el juego de la serpiente
void registers();     // Muestra el estado de los registros
void invalid();  // Maneja el caso de operación inválida
void divx0();        // Maneja la excepción de división por cero
void zoomIn();       // Realiza el acercamiento
void zoomOut();      // Realiza el alejamiento
void actualTime();         // Muestra el tiempo actual
void actualDate();         // Muestra la fecha de hoy
extern void invalidOperation();
void agro(); // imprime escudo de un equipo de futbol
void testMM(); // testea el memory manager
void memoryDump(); // imprime el estado de la memoria
void ps(); // imprime los procesos en ejecución
void testProcesses(); // Testea la creación de procesos
void testPriorities(); // Testea cambio de prioridades
#endif // COMMANDS_H
