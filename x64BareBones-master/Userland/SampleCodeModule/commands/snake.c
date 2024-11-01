#include <commands.h>

#include <snake.h>
#include <syscalls.h>
#include <library.h>
// QUE FUNCIONES FALTAN CORREGIR Y HACER 
// keyInput
// spawnSnake
// BEEP -> SONIDOOOO

typedef struct direcs{
    int x;
    int y;

}direcs;


#define MAX 30

// https://hexdocs.pm/color_palette/ansi_color_codes.html
//typedef enum Colors {  colorsEnum.GREEN = 1, GREEN=1, RED, YELLOW, BLUE };
//#define COLORS 5

// --------------- SNAKE STRUCT ------------------------

typedef enum{ UP = 0, DOWN, LEFT, RIGHT } lastMoveEnum;

typedef struct ColorsStruct{
    int colorAscii;
    char * color;
}ColorsStruct;

typedef struct snakeStruct{

    direcs head;
    direcs body[MAX];
    lastMoveEnum lastMove;
    int bodyDim; // bodyDim + 1 -> total points
    ColorsStruct color; 
    int id;

}snakeStruct;


// --------------- APPLE STRUCT -------------------------------------

typedef struct appleStruct{
    direcs cord;
    char * color; // roja por defecto
}appleStruct;
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
        c1 = getChar();
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
        game = getChar();
    }
    return game - CERO_ASCII; // devuelve en numero en int
}

static void copyColor(snakeStruct * s, int num){
    strcpy(s->color.color, colorNames[num-1]);
    s->color.colorAscii = colorHexa[num-1];
}

// creo que se puede hacer de mejor manera
static void chooseColors(snakeStruct * s1, snakeStruct * s2, int * flag){
    printf("Player 1: choose your snake color\n");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4\n");
    int color1 = getc1();
    copyColor(s1, color1);
    if( *flag == PLAYER_2){

    printf("Player 2: choose your snake color:\n");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4\n");
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

// --------------------------- IMPORTANT MAIN ----------------------
void start(){

    int flag = welcomeSnake();
    snakeStruct * snake1, * snake2; // snake2 asi me tiraba error ---- CHEQUEAR
    chooseColors(&snake1, &snake2, &flag);
    printf("Player 1 chose color: %s\n", snake1->color);
    if( flag == 2){
    printf("Player 2 chose color: %s\n", snake1->color);
    }
    // tendria que hacer una matriz con snake1.color y mandarle el color de la snake

    board(); // mapa para las snakes
    spawnSnake(snake1, snake2, &flag); // fija en que posicion van a spawnear las snakes

    sys_clearScreen();

    startGame(&flag); // gameStarter();
    int start = 0;
    tablero(&flag, start); // pointsTab(0, 0);
    appleStruct * apple ;
    get_apple(&apple); // createFood();
    
    // GAME STARTS
    putBoard(&snake1, &snake2); // printBoard(&snakeP1,&snakeP2);
    putApple(&apple); // updateBoardFromFood();
    
    sleep(); // chequear

    game(&snake1, &snake2, &apple, &flag); // WHILE PARA ABAJO
}

// ------------------ GAME -----------------------
static char finish = 0;
#define EXIT 1


static void handlePlayerInput

// --------------- CONTROLS -----------------------
// PLAYER UNO FLECHITAS
// PLAYER DOS -> W A S D 

// Define para las teclas
#define KEY_ESC 27        // Tecla ESC
#define KEY_W   'w'       // Tecla W (mover hacia arriba)
#define KEY_A   'a'       // Tecla A (mover hacia la izquierda)
#define KEY_S   's'       // Tecla S (mover hacia abajo)
#define KEY_D   'd'       // Tecla D (mover hacia la derecha)

// Define para las flechas (código de escape + código de flecha)
#define KEY_UP    '\033[A' // Flecha arriba
#define KEY_DOWN  '\033[B' // Flecha abajo
#define KEY_LEFT  '\033[D' // Flecha izquierda
#define KEY_RIGHT '\033[C' // Flecha derecha
#define APPLE '*' // CHEQUEAR


static int checkSnake(struct snake * s1){
    int flag=1;
    if (s->head.x < 0 || s->head.x >= X_SQUARES || s->head.y < 0 || s->head.y >= Y_SQUARES) {
    //call_beep(2000); 
    //call_beep(1000);
    // call_beep(500); SONIDOOOOOO
    return flag;

}
// Verificar colisión con el cuerpo o con cualquier otra cosa que no sea vacío o comida
    if (boardMatrix[s->head.y][s->head.x] != CER0 && boardMatrix[s->head.y][s->head.x] != APPLE) {
    //call_beep(500);
    //call_beep(300);
    // call_beep(100);
    return flag;
    }
return !flag;
}


static int snakeEnDir(struct snake * s1, lastMoveEnum move){
    static int dir[2][8] = {{-1, -1, -1, 0, 1, 1, 1, 0},
                            {-1, 0, 1, 1, 1, 0, -1, -1}};
    int flag=1;
    switch (move) {
        case UP:
            s1->head.x = dir[0][0]; // Movimiento en X
            s1->head.y = dir[1][0]; // Movimiento en Y
            if( checkSnake(s1)){
                return flag;
            }
            break;
        case RIGHT:
            s1->head.x = dir[0][2];
            s1->head.y = dir[1][2];
            if( checkSnake(s1)){
                return flag;
            }
            break;
        case DOWN:
            s1->head.x = dir[0][4];
            s1->head.y = dir[1][4];
            if( checkSnake(s1)){
                return flag;
            }
            break;
        case LEFT:
            s1->head.x = dir[0][6];
            s2->head.y = dir[1][6];
            if( checkSnake(s1)){
                return flag;
            }
            break;
        default:
            return -1; // Movimiento no válido
    }if( )
}

void keyInput(struct snake * s1, struct snake * s2){
    char flag=0;
    if( s2 == NULL){
        flag=1; // one player
    }
    if( s2 != NULL)
        flag = 2; // two player
    char c = getChar();
    char moveS1, moveS2;
     if (flag == 2) { // Usar todas las teclas (W, A, S, D y flechas)
        switch (c) {
            case KEY_ESC:
                finish = 1
                return; 
        // ---------------------------- P1 ->  PLAYER 2 MOVEMENTS -----------------------------
            case KEY_W:
                if (s1->lastMove != DOWN) {
                    moveS1 = snakeEnDir(s1, UP); 
                }
                break;

            case KEY_A:
                if (s1->lastMove != RIGHT) {
                    l_move = snakeEnDir(s1, LEFT); 
                }
                break;

            case KEY_S:
                if (s1->lastMove != UP) {
                    l_move = snakeEnDir(s1, DOWN); 
                    
                }
                break;

            case KEY_D:
                if (s1->lastMove != LEFT) {
                    l_move = snakeEnDir(s1, RIGHT); 
            // ---------------------------- P2 ->  PLAYER 1 MOVEMENTS -----------------------------
                }
                break;

            case KEY_UP:
                if (s1->lastMove != DOWN) {
                    l_move = snakeEnDir(s1, UP); 
                }
                break;

            case KEY_DOWN:
                if (s1->lastMove != UP) {
                    l_move = snakeEnDir(s1, DOWN); 
                   
                }
                break;

            case KEY_LEFT:
                if (s1->lastMove != RIGHT) {
                    l_move = snakeEnDir(s1, LEFT); 
             
                }
                break;

            case KEY_RIGHT:
                if (s1->lastMove != LEFT) {
                    l_move = snakeEnDir(s1, RIGHT);
                }
                break;
            default:
                lost = snakeEnDir(s1, s1->lastMove);
                lost = snakeEnDir(s2, s2->lastMove);
                break;
        }
        // ---------------------------- P1 ->  PLAYER 1 MOVEMENTS -----------------------------
    } else if( flag == 1) { // Solo usar las teclas de flecha
        switch (c) {
            case KEY_ESC:
                finish = 1
                return;
            case KEY_UP:
                if (s1->lastMove != DOWN) {
                    moveS1 = snakeEnDir(s1, UP);
                    
                }
                break;

            case KEY_DOWN:
                if (s1->lastMove != UP) {
                    moveS1 = snakeEnDir(s1, DOWN); 
                }
                break;

            case KEY_LEFT:
                if (s1->lastMove != RIGHT) {
                    moveS1 = snakeEnDir(s1, LEFT); 
                }
                break;

            case KEY_RIGHT:
                if (s1->lastMove != LEFT) {
                    moveS1 = snakeEnDir(s1, RIGHT);
                }
                break;
            default:
                moveS1 = snakeEnDir(s1, s1->lastMove);
                break;
        }
    }else{
        moveS1 = moveSnake(s1, s1->lastMove);
        moveS2 = moveSnake(s2, s2->lastMove);
    }
if( flag == 2){
    if 
    
}
if( flag == 1){
    return moveS1;
}

}
    


static void playersKeyboard(int *flag,struct snake * s1,struct snake * s2, int *w1, int *w2){
    if(*flag == 1){
        *w1 = keyInput(&s1,NULL);
        status(&s1, NULL)
    }else{
        *w2 = keyInput(&s1,&s2);
        status(&s1, &s2);
    }
return;
}


void checkGameLost(int *flag,int * w1, int *w2,struct snake * s1,struct snake * s2 ){
    
}

static void game(struct snake * s1, struct snake * s2, struct snake * apple, int * flag){
    int w1, w2;
    while( finish != EXIT){
        playersKeyboard(&flag, &s1, &s2, &w1, &w2);
    
        updateBoard(&snakeP1, &snakeP2);
        printBoard(&snakeP1, &snakeP2);   // FUNCION IMPORTANT  
        call_sleep(100);
    
        checkGameLost(&flag, &w1, &w2, &snakeP1, &snakeP2);
    }
}


// ---------------- APPLE -------------

// tiene que ir en una libreria 
static unsigned long int next = 1;

int rand(void) 
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

void srand(unsigned int seed)
{
    next = seed;
}


void get_apple(appleStruct * apple){
    apple->color = 0xaf0000; // APPLE RED COLOR
    direcs direc;
    direc.x = rand() // insert number
    direc.y = rand();
    while( board[direc.x][direc.y] != CER0){
        direc.x = rand() % 32; // insert number
        direc.y = rand() % 24; // chequear
    }
    
    apple->cord.x = direc.x;
    apple->cord.y = direc.y;
    
}


void putApple(appleStruct * snake){
    sys_drawRectangle(apple->color,(uint64_t)apple->cord.x, (uint64_t)apple->cord.y,  , );
}
// ----------------------- STARTER ---------------------------- 

static void check(int *flag){
    char c;
    if( *flag == 1){
        printf("Player 1, Did you check? y/n\n");
        while (c != 'y') {
            c = getChar();
        }
    }
    if( *flag == 2){
        printf("Players, Did you check? y/n \n");
        while (c != 'y') {
            c = getChar();
        }
    }
    return;
}

static void startGame(int * flag){
    printf("turn off CAPS-LOCK \t\t");
    char c;
    check(&flag);
    sys_clearScreen();
    if( *flag == 1){
        printf("Player 1, are you ready?\n");
    }
    if( *flag == 2){
        printf("Players, are you ready?\n");
    }
    for( int i=5; i > 0; i--)
        printf("Game starts in %d", i);
    sys_sleep(); // chequear
    sys_delete(); // chequear
}



static tablero(int * flag, int start){
    // syscall para donde poner el tablero
    if( * flag == 1){
        printf("Points: %d\n", start);
    }
    if( *flag == 2){
        printf("P1 vs P2");
        printf(" P1: %d", start);
        printf(" P2: %d", start);
    }
}
// ----------------------- MAKE SNAKE --------------------------------

#define P1 1
#define P2 2
/*void makeSnake(snakeStruct * s1, snakeStruct * s2, int *flag){
    s1->id = P1;
    s1->head.x = 
}*/

// crea la snake y la posiciona en una zona del tablero
// el cual seria p1 medio a la izq
// p2 medio a la derecha
static void spawnSnake(snakeStruct * s1, snakeStruct * s2, int *flag){


}