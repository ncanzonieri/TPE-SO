#include <snake.h>
#include <string.h>


typedef struct direcs{
    int x;
    int y;

}direcs;


#define MAX 30

// https://hexdocs.pm/color_palette/ansi_color_codes.html
typedef enum Colors {  GREEN=1, RED, YELLOW, BLUE };
#define COLORS 5



typedef struct ColorsStruct{
    int colorAscii;
    char * color;
}ColorsStruct;

typedef struct snakeStruct{

    direcs head;
    direcs body[MAX];
    int bodyDim; // bodyDim + 1 -> total points
    ColorsStruct color; 
    direcs tail; // esta se va 'eliminando'

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
const int colorHexa[COLORS_COUNT] = { 0x000080, 0x008000, 0xff0000, 0xffff00 };


static int welcomeSnake(){
    printf("Welcome to Snake\n");
    printf("Number of players: 1 o 2");
    char game;
    while (game != '1' && game != '2') {
        game = getchar();
    }
    return game - CERO_ASCII; // devuelve en numero en int
}

// creo que se puede hacer de mejor manera
static void chooseColors(snakeStruct * s1, snakeStruct * s2, int * flag){
    printf("Player 1: choose your snake color");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4");
    int color1 = getc1();
    strcpy(s1->color.color, colorNames[color1-1]);
    s1->color.colorAscii = colorHexa[color1-1];
    if( *flag == PLAYER_2){
    printf("Player 2: choose your snake color:");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4");
    
    int color2 = getc2(color1);
    strcpy(s2->color.color, colorNames[color2-1]);
    s2->color.colorAscii = colorHexa[color2-1];
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

    board(&flag); 

}

