
#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>
#include <syscalls.h>
#include <library.h>


// --------- DEFINES -------------
#define BOARD_WIDTH 32
#define BOARD_HEIGHT 24
#define MIN_SNAKE_LENGTH 5
#define MAX_SNAKE_LENGTH 100

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
    uint32_t colorAscii;
    char * color;
}ColorsStruct;

typedef struct snakeStruct{

    direcs head;
    direcs body[MAX_SNAKE_LENGTH];
    lastMoveEnum lastMove;
    int bodyDim; // bodyDim + 1 -> total points
    char id;
    int player;
    int points;
    uint32_t color; 

}snakeStruct;

typedef struct appleStruct{
    direcs cord;
    uint32_t color; // roja por defecto
    char id;
}appleStruct;

// Funciones del  Snake
void snake();
void start();
int welcomeSnake();

void board();
void spawnSnake(snakeStruct *s);
void makeApple();
void printAppleInBoard();

void printSnakeInBoard(snakeStruct *s[]);

int keyPlayerOne(snakeStruct *snake1);
int keyPlayerTwo(snakeStruct *snake1, snakeStruct *snake2);


int snakeEnDir(snakeStruct *s, lastMoveEnum move);
int crash(snakeStruct *s);
int snakeEatsApple(snakeStruct *s);
void deleteTail(snakeStruct *s);
void refreshSnakesInBoard( snakeStruct * s[]);


void winner(int *w, snakeStruct *s1, snakeStruct *s2);

// Funciones del jugador



#endif // SNAKE_H

