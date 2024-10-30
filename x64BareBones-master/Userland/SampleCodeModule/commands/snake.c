#include <commands.h>

#include <snake.h>
#include <string.h>


typedef struct direcs{
    int x;
    int y;

}direcs;


#define MAX 30

// https://hexdocs.pm/color_palette/ansi_color_codes.html
//typedef enum Colors {  colorsEnum.GREEN = 1, GREEN=1, RED, YELLOW, BLUE };
//#define COLORS 5



typedef struct ColorsStruct{
    int colorAscii;
    char * color;
}ColorsStruct;

typedef struct snakeStruct{

    direcs head;
    direcs body[MAX];
    direcs tail; // esta se va 'eliminando'
    int bodyDim; // bodyDim + 1 -> total points
    ColorsStruct color; 
    int id;

}snakeStruct;

// typedef struct _snake * snakeStruct;


// ----------  MAIN SNAKE --------------
void snake(){
    start();
    exitSnake();
    return;
}

#define CERO_ASCII '0'
#define PLAYER_2 2
// ----------------- MENU & THEME -----------------




// ----------------- GETTERS -------------------

static int getc1(){
    char c1;
    while (c1 != '1' && c1 != '2' && c1 != '3' && c1 != '4') {
        c1 = getchar();
    }
}

static int getc2(int c1){
    char c2;
    do {
        c2 = getc1();
    } while (c2 == c1);

    return c2 - CERO_ASCII;
}

// ----- en snake.h meto los colors y eso

#define COLORS_COUNT 4

const char *colorNames[COLORS_COUNT] = { "GREEN", "RED", "YELLOW","BLUE" };
const uint32_t colorHexa[COLORS_COUNT] = { 0x000080, 0x008000, 0xff0000, 0xffff00 };


static int welcomeSnake(){
    printf("Welcome to Snake\n");
    printf("Number of players: 1 o 2");
    char game;
    while (game != '1' && game != '2') {
        game = getchar();
    }
    return game - CERO_ASCII; // devuelve en numero en int
}

static void copyColor(snakeStruct * s, int num){
    strcpy(s->color.color, colorNames[num-1]);
    s->color.colorAscii = colorHexa[num-1];
}

// creo que se puede hacer de mejor manera
static void chooseColors(snakeStruct * s1, snakeStruct * s2, int * flag){
    printf("Player 1: choose your snake color");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4");
    int color1 = getc1();
    copyColor(s1, color1);
    if( *flag == PLAYER_2){

    printf("Player 2: choose your snake color:");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4");
    copyColor(s2, getc2(color1));

    }
    return;
}
// -------------- BOARD --------------

#define X 30
#define Y 30// dps hay que def esto
#define CER0 '\0'

static char boardMatrix[X][Y];

static void board(){
      for(int i = 0; i <  Y; i++){
        for(int j = 0; j < X; j++){
            boardMatrix[i][j] = CER0;
        }
    }
    return;
}



static void spawnSnake(snakeStruct * s1, snakeStruct * s2, int *flag){
    makeSnake(s1, s2);
    if( *flag == 2){
        makeSnake(s1, s2);
    }
return;
}


void start(){

    int flag = welcomeSnake();
    snakeStruct * snake1, * snake2; // snake2 asi me tiraba error ---- CHEQUEAR
    chooseColors(snake1, snake2, &flag);
    printf("Player 1 chose color: %s", snake1->color);
    if( flag == 2){
    printf("Player 2 chose color: %s", snake1->color);
    }
    // tendria que hacer una matriz con snake1.color y mandarle el color de la snake

    board(); // mapa para las snakes
    spawnSnake(snake1, snake2, &flag);

    clearScreen(); // chequear eso

}




// ----------------------- MAKE SNAKE --------------------------------

#define P1 1
#define P2 2
/*void makeSnake(snakeStruct * s1, snakeStruct * s2, int *flag){
    s1->id = P1;
    s1->head.x = 
}*/