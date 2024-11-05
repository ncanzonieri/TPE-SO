
#include <commands.h>
#include <snake.h>
#include <syscalls.h>
#include <library.h>
#define MAX 30
#define CERO_ASCII '0'
#define PLAYER_2 2
#define CER0 '\0'
#define MIN_DIM 5
#define P1 '#'
#define P2 '@'
#define APPLE '*'
#define NULL ((void*)0) 

// chequear //pixel positions
#define Y_MAX 768
#define X_MAX 1024
#define PIXEL_POS_X(i) (i) * SQUARE_SIZE
#define PIXEL_POS_Y(i) (i + 1) * SQUARE_SIZE

//board positions
#define SQUARE_SIZE 32
#define X_SQUARES (X_MAX / SQUARE_SIZE) // 32
#define Y_SQUARES 23 // 24

static char boardMatrix[X_SQUARES][Y_SQUARES];
static int flagPlayers=1;



static int getc1();
static int getc2(int c1);
static void spawnSnake(snakeStruct *s, snakeStruct *s2);

static void copyColor(snakeStruct * s, int num);

static void chooseColors(snakeStruct * s1, snakeStruct * s2);

static void board();

static void reset(int xPos, int yPos);

static int checkSnake(struct snakeStruct * s);

static int snakeEnDir(struct snakeStruct * s1, lastMoveEnum move);

static void deleteTail(struct snakeStruct * s);

static int checkChar(char c);

static int keyPlayerOne(snakeStruct *s);

static int keyPlayerTwo(snakeStruct * s1, snakeStruct * s2);

static void winner(int * w1, int *w2, struct snakeStruct *s1, struct snakeStruct *s2);

static void get_apple();

static void putApple();

void updateBoard(snakeStruct * s1, snakeStruct * s2); 

static void updateBoardFromSnake(snakeStruct * s);

static void updateBoardApple();

static void putSnakeInBoard(snakeStruct * s1, snakeStruct *s2);


static appleStruct * apple;


static char finish = 0;
#define EXIT 1


void snake(){
    sys_clearScreen();
    start(); 
    sys_clearScreen(); // finish = 1 y sale de start();
    return;
}

void start(){
    int flagPlayers = welcomeSnake();
    snakeStruct * snake1, * snake2; 
    chooseColors(&snake1, &snake2);
    printf("Player 1 chose color: %s\n", snake1->color);
    if( flagPlayers == 2){
        printf("Player 2 chose color: %s\n", snake1->color);
    }
    
    sys_clearScreen();

    board(); // mapa para las snakes
    
    spawnSnake(&snake1, &snake2); // fija en que posicion van a spawnear las snakes
    get_apple(); // createFood();
    updateBoardApple();


    putSnakeInBoard(&snake1, &snake2); // printBoard();
    putApple(); 
    
    sys_sleep(1000); 

    int w1,w2;
    while( finish != EXIT){
        if(flagPlayers == 1){
            w1 = keyPlayerOne(&snake1); // arranque haciendola toda junta pero me termine mareando y no llegue a nada
        }else{
            w2 = keyPlayerTwo(&snake1,&snake2); // podria volver que si w2 -> gano p2 y w2=1 -> gano p1
        }   
        updateBoard(&snake1, &snake2); 
        putSnakeInBoard(&snake1, &snake2); // printBoard()
    }
    winner(&w1,&w2,&snake1, &snake2);

    }


// ----------------------------------- WELCOME ---------------------
int welcomeSnake(){
    printf("Welcome to Snake\n");
    printf("Number of players: 1 o 2\n");
    char game;
    while (game != '1' && game != '2') {
        game = getChar();
    }
    return game - CERO_ASCII; // devuelve en numero en int
}

void updateBoard(snakeStruct * s1, snakeStruct * s2){
    updateBoardFromSnake(s1);
    updateBoardFromSnake(s2);
}

// ------------------------------- UPDATESSSS ---------------------------

static void updateBoardFromSnake(snakeStruct * s) {
    if (s == NULL) {
        return;
    }
    for (int i = 0; i < s->bodyDim; i++) {
        boardMatrix[s->body[i].y][s->body[i].x] = s->id;
    }
}

static void updateBoardApple() {
    boardMatrix[apple->cord.y][apple->cord.x] = APPLE;
}



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


// ----------------------- MAKE SNAKE --------------------------------


static void spawnSnake(snakeStruct *s, snakeStruct *s2) {
    // Inicialización para el Jugador 1
    if (flagPlayers == 1) {
        s->id = P1;
        s->head.x = MIN_DIM- 1;
        s->head.y = Y_SQUARES / 2 + 1;
        for (int i = 0; i <= s->head.x; i++) { // La cola (x, y) es el primer elemento del array "body".
            s->body[i].x = i;
            s->body[i].y = s->head.y;
            boardMatrix[s->head.y][i] = s->id;
        }
        s->bodyDim = MIN_DIM;
        s->lastMove = RIGHT;
        s->points = 0;
        boardMatrix[s->head.y][s->head.x] = s->id;
        return;
    }
    // Inicialización para el Jugador 2
    s2->id = P2;
    s2->head.x = X_SQUARES - MIN_DIM;
    s2->head.y = Y_SQUARES / 2 - 1;
    // Primer bucle para inicializar la posición x de cada segmento del cuerpo
    for (int i = X_SQUARES - 1; i >= s2->head.x; i--) {
        for (int j = 0; j < MIN_DIM; j++) {
        s2->body[j].x = i;
        s2->body[j].y = s->head.y;
        boardMatrix[s2->head.y][s2->body[j].x] = s->id;
        }
    }
    s2->bodyDim = MIN_DIM;
    s2->lastMove = LEFT;
    s2->points = 0;
    boardMatrix[s2->head.y][s2->head.x] = s2->id;

}

static void copyColor(snakeStruct * s, int num){
    strcpy(s->color.color, colorNames[num-1]);
    s->color.colorAscii = colorHexa[num-1];
}

// creo que se puede hacer de mejor manera
static void chooseColors(snakeStruct * s1, snakeStruct * s2){
    printf("Player 1: choose your snake color\n");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4\n");
    int color1 = getc1();
    copyColor(s1, color1);
    if( flagPlayers == 2){

    printf("Player 2: choose your snake color:\n");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4\n");
    copyColor(s2, getc2(color1));

    }
return;
}

// -------------- BOARD --------------


static void board(){
    for(int i = 0; i <  Y_SQUARES; i++){
        for(int j = 0; j < X_SQUARES; j++){
            boardMatrix[i][j] = CER0;
        }
    }
return;
}


static void putSnakeInBoard(snakeStruct * s1, snakeStruct *s2){
    for( int i=0; i<X_MAX; i++){
        for( int j=0; j<Y_MAX; j++){
            if( boardMatrix[i][j] == P1){ // s1->id o P1 el define
                sys_drawRectangle(s1->color.colorAscii, PIXEL_POS_X(j), PIXEL_POS_Y(i), SQUARE_SIZE, SQUARE_SIZE);
            }
            else if( boardMatrix[i][j] == P2){
                    sys_drawRectangle(s2->color.colorAscii, PIXEL_POS_X(j), PIXEL_POS_Y(i), SQUARE_SIZE, SQUARE_SIZE);
            }
            else if( boardMatrix[i][j] == APPLE){
                putApple();
            }else{
                reset(j,i);
            }
        }
    }
}
    
static void reset(int xPos, int yPos){
    if( (xPos % 2 && yPos % 2) || (xPos % 2 == 0 && yPos % 2 == 0)){
        sys_drawRectangle(0x000000, PIXEL_POS_X(xPos), PIXEL_POS_Y(yPos), SQUARE_SIZE,SQUARE_SIZE);
    }else{
        sys_drawRectangle(0xFFFFFF, PIXEL_POS_X(xPos), PIXEL_POS_Y(yPos), SQUARE_SIZE,SQUARE_SIZE);
    }
}




static int checkSnake(struct snakeStruct * s){
    int flag=1;
    if (s->head.x < 0 || s->head.x >= X_SQUARES || s->head.y < 0 || s->head.y >= Y_SQUARES) {
    return flag;

}
// Verificar colisión con el cuerpo o con cualquier otra cosa que no sea vacío o comida
    if (boardMatrix[s->head.y][s->head.x] != CER0 && boardMatrix[s->head.y][s->head.x] != APPLE) {
        return flag;
    }
return !flag;
}


static int snakeEnDir(struct snakeStruct * s1, lastMoveEnum move){
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
            s1->head.y = dir[1][6];
            if( checkSnake(s1)){
                return flag;
            }
            break;
        default:
            return -1; // Movimiento no válido
    }if( checkSnake(s1)){
        s1->points +=1;
        s1->bodyDim +=1;
        s1->body[s1->bodyDim-1] = s1->head;
        return 0;   
    }
deleteTail(s1);
return !flag;
}

static void deleteTail(struct snakeStruct * s){
    boardMatrix[s->body[0].x][s->body[0].y] = CER0;
    int i=1;
    while( i < s->bodyDim){
        s->body[i - 1] = s->body[i];
        i++;
    }
    s->body[i - 1] = s->head;;
}


static int checkChar(char c) {
    return c >= 'a' && c <= 'z';
}

// CONTROLES DE TECLADO

// osea si retorna 1 sigue estando 
static int keyPlayerOne(snakeStruct *s){
    int flag=1;
    char c=getChar();
    if( checkChar(c)){
        if(flag){
            switch (c){
                case KEY_ESC:
                    finish = END;
                    break;
                case KEY_W:
                    if( s->lastMove != DOWN)
                        flag = snakeEnDir(s, UP);
                    break;
                case KEY_S:
                    if( s->lastMove != UP)
                        flag = snakeEnDir(s, DOWN);
                case KEY_A:
                    if( s->lastMove != RIGHT)
                        flag = snakeEnDir(s, LEFT);
                case KEY_D:
                    if( s->lastMove != LEFT)
                        flag = snakeEnDir(s, RIGHT);
                default:
                    flag = snakeEnDir(s,s->lastMove);
                    break;
            }
        }else{
            flag = snakeEnDir(s,s->lastMove);
        }
    } // osea si flag = 0 es que choco
    
return flag;

}


static int keyPlayerTwo(snakeStruct * s1, snakeStruct * s2) {
    int flagS1 = 0, aux = 0;
    char c = getChar();
    if (checkChar(c)) {
        switch (c) {
            case KEY_ESC: // Quit game (ESC)
                finish = 1;
                break;
            case KEY_W: // Move up for player 1
                if (s1->lastMove != DOWN) {
                    flagS1 = snakeEnDir(s1, UP);
                    aux = snakeEnDir(s2, s2->lastMove);
                    flagS1 = (aux > flagS1) ? aux : flagS1;
                }
                break;
            case KEY_S: // Move down for player 1
                if (s1->lastMove != UP) {
                    flagS1 = snakeEnDir(s1, DOWN);
                    aux = snakeEnDir(s2, s2->lastMove);
                    flagS1 = (aux > flagS1) ? aux : flagS1;
                }
                break;
            case KEY_A: // Move left for player 1
                if (s1->lastMove != RIGHT) {
                    flagS1 = snakeEnDir(s1, LEFT);
                    aux = snakeEnDir(s2, s2->lastMove);
                    flagS1 = (aux > flagS1) ? aux : flagS1;
                }
                break;
            case KEY_D: // Move right for player 1
                if (s1->lastMove != LEFT) {
                    flagS1 = snakeEnDir(s1, RIGHT);
                    aux = snakeEnDir(s2, s2->lastMove);
                    flagS1 = (aux > flagS1) ? aux : flagS1;
                }
                break;
            case KEY_I: // Move up for player 2
                if (s2->lastMove != DOWN) {
                    flagS1 = snakeEnDir(s2, UP) ? 2 : 0;
                    aux = snakeEnDir(s1, s1->lastMove);
                    flagS1 = (aux > flagS1) ? aux : flagS1;
                }
                break;
            case KEY_K: // Move down for player 2
                if (s2->lastMove != UP) {
                    flagS1 = snakeEnDir(s2, DOWN) ? 2 : 0;
                    aux = snakeEnDir(s1, s1->lastMove);
                    flagS1 = (aux > flagS1) ? aux : flagS1;
                }
                break;
            case KEY_J: // Move left for player 2
                if (s2->lastMove != RIGHT) {
                    flagS1 = snakeEnDir(s2, LEFT) ? 2 : 0;
                    aux = snakeEnDir(s1, s1->lastMove);
                    flagS1 = (aux > flagS1) ? aux : flagS1;
                }
                break;
            case KEY_L: // Move right for player 2
                if (s2->lastMove != LEFT) {
                    flagS1 = snakeEnDir(s2, RIGHT) ? 2 : 0;
                    aux = snakeEnDir(s1, s1->lastMove);
                    flagS1 = (aux > flagS1) ? aux : flagS1;
                }
                break;
            default: // Default movement
                aux = snakeEnDir(s1, s1->lastMove);
                flagS1 = snakeEnDir(s2, s2->lastMove);
                flagS1 = (aux > flagS1) ? aux : flagS1;
                break;
        }
    } else { // Continue with the last moves if no input
        aux = snakeEnDir(s1, s1->lastMove);
        flagS1 = snakeEnDir(s2, s2->lastMove);
        flagS1 = (aux > flagS1) ? aux : flagS1;
    }

    return flagS1;
}


// ---------------- APPLE -------------

static void get_apple(){
    apple->color.colorAscii = 0xaf0000; // APPLE RED COLOR
    apple->id = APPLE;
    direcs direc;
    direc.x = rand() % 32;  // insert number
    direc.y = rand() % 24;
    while( boardMatrix[direc.x][direc.y] != CER0){
        direc.x = rand() % 32; // insert number
        direc.y = rand() % 24; // chequear
    }
    
    apple->cord.x = direc.x;
    apple->cord.y = direc.y;

}

static void putApple(){
    sys_drawRectangle(apple->color.colorAscii, PIXEL_POS_X((uint64_t)apple->cord.x), PIXEL_POS_Y((uint64_t)apple->cord.y), SQUARE_SIZE, SQUARE_SIZE);
}

// ------------ WINNER -------------------
static void winner(int * w1, int *w2, struct snakeStruct *s1, struct snakeStruct *s2){
    if( *w1 == 1 && flagPlayers == 1){
        printf("Player 1 points: %d\n", s1->points);
    }
    if( flagPlayers == 2 ){
        if( *w2 == 2){
            printf("Player 2 won\n!");
            printf("%d vs %d", s2->points, s1->points);
        }
        if( *w2 == 1){
            printf("Player 1 won\n!");
            printf("%d vs %d", s1->points, s2->points);
        }
    }
    printf("Ready for another game? Press y\n");
    char c = getChar();
    if( c == 'y' ){
        start();
    }else
        finish=1;
return;

}

