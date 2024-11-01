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
    int id;
    int points;
    ColorsStruct color; 

}snakeStruct;

typedef struct appleStruct{
    direcs cord;
    char * color; // roja por defecto
}appleStruct;

// Funciones del  Snake
void snake();
int welcomeSnake();
void startSnake();
void exitSnake();


#endif // SNAKE_H
