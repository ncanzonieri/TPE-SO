
#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>
#include <syscalls.h>
#include <library.h>

// --------- DEFINES -------------
#define BOARD_WIDTH 32
#define BOARD_HEIGHT 24
#define INITIAL_SNAKE_LENGTH 3
#define MAX_SNAKE_LENGTH 30

//--------------- Paleta de colores ------------------

#define COLORS_COUNT 4


// --------------- DEFINE LETRAS ---------------------------
// Define para las teclas
#define END 1
#define KEY_ESC 27        
#define KEY_W   'w'      
#define KEY_A   'a'      
#define KEY_S   's'       
#define KEY_D   'd'       
// P2
#define KEY_H 'h' 
#define KEY_J 'j' 
#define KEY_K 'k' 
#define KEY_U 'u' 
#define KEY_L 'l'
#define KEY_I 'i'


#define APPLE_GREEN 0x87af00

const char *colorNames[COLORS_COUNT] = { "GREEN", "RED", "YELLOW","BLUE" };
const uint32_t colorHexa[COLORS_COUNT] = { 0x000080, 0x008000, 0xFF0000, 0xFFFF00 };

// Direcciones de movimiento
typedef enum { UP=0, DOWN, LEFT, RIGHT} lastMoveEnum;

// -------------- STRUCTS -----------------

typedef struct direcs{
    int x;
    int y;

}direcs;


typedef struct ColorsStruct{
    int colorAscii;
    char * color;
}ColorsStruct;

typedef struct snakeStruct{

    direcs head;
    direcs body[MAX_SNAKE_LENGTH];
    lastMoveEnum lastMove;
    int bodyDim; // bodyDim + 1 -> total points
    char id;
    int points;
    ColorsStruct color; 

}snakeStruct;

typedef struct appleStruct{
    direcs cord;
    ColorsStruct color; // roja por defecto
    char id;
}appleStruct;

// Funciones del  Snake
void snake();
int welcomeSnake();
void start();
void exitSnake();


#endif // SNAKE_H

