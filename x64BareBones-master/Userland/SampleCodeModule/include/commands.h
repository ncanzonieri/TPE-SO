
#ifndef COMMANDS_H  // Prevenir la inclusión múltiple
#define COMMANDS_H

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

#endif // COMMANDS_H
