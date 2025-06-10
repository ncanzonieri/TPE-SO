// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <snake.h>
#include <commands.h>
#include <syscalls.h>
#include <library.h>


// defines 
#define MAX 30
#define PLAYER_2 2
#define CERO '0'
#define BEATS 100


#define P1_ID '#'
#define P2_ID '@'
#define APPLE '*'
#define NULL ((void*)0) 

// chequear //pixel positions
#define Y_MAX 768
#define X_MAX 1024
#define PIXEL_POS_X(i) (i) * SQUARE_SIZE
#define PIXEL_POS_Y(i) (i) * SQUARE_SIZE

//board positions
#define SQUARE_SIZE 32
#define X_SQUARES (X_MAX / SQUARE_SIZE) // 32
#define Y_SQUARES (Y_MAX / SQUARE_SIZE) // 24

static char boardMatrix[X_SQUARES][Y_SQUARES];
static int flagPlayers=1;
static int exit=0;


static appleStruct * apple;


// chequear chiche 
static void reset(int xPos, int yPos);
static void startTitle();


int snake(int argc, char *argv[]){
    // sys_clearScreen();
    start(); 
    sys_clearScreen(); // finish = 1 y sale de start();
    return 0;
}

#define P1_DIM 1
#define P2_DIM 2
#define EXIT 1

static void startTitle(){ 
    char* details[]={"Player 1:\n\tMoves with W A S D\n\tStarts in the left\n",
    "Player 2:\n\tMoves with I J K L\n\tStarts in the right\n"};
    for(int i=0; i<flagPlayers; i++){
        printf("%s",details[i]);
    }
    printf("\t\t\t\tSnake game starts in 3...");
    sys_sleep(1000);
    printf("\t2...");
    sys_sleep(1000);
    printf("\t1...");
    sys_sleep(1000);
    sys_clearScreen();

}



void start(){
    exit = 0; // cada vez que arranco se reinicia 
    sys_clearScreen();
    flagPlayers = welcomeSnake();
    snakeStruct * snakes[flagPlayers]; 
    snakeStruct snake1,snake2; 
    snake1.player = 1;
    snake2.player = 2;
    snakes[0]=&snake1;
    snakes[1]=&snake2;

    startTitle();        
    
    board();
    for(int i=0; i<flagPlayers; i++){
        spawnSnake(snakes[i]);
    }

    makeApple();
   // printSnakeInBoard(&snake1, &snake2);

    printSnakeInBoard(snakes);

    sys_sleep(BEATS);

    int w=0;
    while (exit != EXIT){
        if( flagPlayers == 1){
            w = keyPlayerOne(&snake1); 
        }else{
            w = keyPlayerTwo(&snake1,&snake2); 
        }   
        refreshSnakesInBoard(snakes);
        printSnakeInBoard(snakes);
        sys_sleep(BEATS);
        
        winner(&w,&snake1, &snake2);
    
    }   

}

void board(){
    for( int i=0; i< X_SQUARES; i++){
        for( int j=0; j< Y_SQUARES; j++){
            boardMatrix[i][j] = CERO;
        }
    }
}

void refreshSnakesInBoard( snakeStruct * s[]){
    for(int j=0; j<flagPlayers; j++){
        for( int i=0; i < s[j]->bodyDim; i++){
            boardMatrix[s[j]->body[i].x][s[j]->body[i].y] = s[j]->id;
        }
    }
}

// snake Funcs 
void spawnSnake(snakeStruct * s){
    if( s->player == P1_DIM){
        s->id = P1_ID;
        s->color = 0x00afd7; // otro azul
        s->head.x = MIN_SNAKE_LENGTH - 1;
        s->head.y = Y_SQUARES/2 +1;
        for(int i = 0; i < MIN_SNAKE_LENGTH; i++){ // Tail (x,y) is the first element of "body" array.
            s->body[i].x = i;
            s->body[i].y = s->head.y;
            boardMatrix[i][s->head.y] = s->id;
        }
        s->lastMove = RIGHT;
    }else
    // p2 snake
    if( s->player == P2_DIM){
        s->id = P2_ID;
        s->color = 0x0000ff; // azul
        s->head.x = X_SQUARES-MIN_SNAKE_LENGTH;
        s->head.y = Y_SQUARES/2 - 1;
        for(int j=0; j < MIN_SNAKE_LENGTH; j++){ // Tail (x,y) is the first element of "body" array.
            s->body[j].x = X_SQUARES-1-j;
            s->body[j].y = s->head.y;
            boardMatrix[X_SQUARES-1-j][s->head.y] = s->id;
        }
        s->lastMove = LEFT;
    }
    s->bodyDim = MIN_SNAKE_LENGTH;
    s->points = 0;
}



void refreshSnakeInBoard(snakeStruct * s){
    if( s == NULL){
        return;
    }
    for( int i=0; i < s->bodyDim; i++){
        boardMatrix[s->body[i].y][s->body[i].x] = s->id;
    }
}


// APPLE FUNCS

void makeApple(){
    apple->color = 0xff0000; // APPLE RED COLOR
    apple->id = APPLE;

    direcs direc;
    do{
        direc.x = ownRand(0,X_SQUARES-1); // insert number
        direc.y = ownRand(1,Y_SQUARES-1); // chequear
    }while( boardMatrix[direc.x][direc.y] != CERO);
    
    apple->cord.x = direc.x;
    apple->cord.y = direc.y;
    boardMatrix[apple->cord.x][apple->cord.y] = APPLE;
}

void printAppleInBoard(){
    sys_drawRectangle(apple->color, PIXEL_POS_X(apple->cord.x), PIXEL_POS_Y(apple->cord.y), SQUARE_SIZE, SQUARE_SIZE);
}

// ---- end apple functs


 void printSnakeInBoard(snakeStruct * s[]){
    for(int i=0; i < Y_SQUARES; i++){
        for( int j=0; j < X_SQUARES; j++){
            if(boardMatrix[j][i]==CERO){
                reset(j,i);
            }else if(boardMatrix[j][i]==APPLE){
                printAppleInBoard();
            }else{
                for(int k=0; k<flagPlayers; k++){
                    if(boardMatrix[j][i]==s[k]->id){
                        sys_drawRectangle(s[k]->color,PIXEL_POS_X(j), PIXEL_POS_Y(i), SQUARE_SIZE, SQUARE_SIZE);
                    }
                }
            }
        }
    }
    //sys_write(1,"printSnake\n",12,0x00ffd7);
}

// chequear chiche



static void reset(int xPos, int yPos) {
    int color = ((xPos % 2 && yPos % 2) || (xPos % 2 == 0 && yPos % 2 == 0)) ? 0x005f00 : 0x00af00; // verde claro y oscuro
    sys_drawRectangle(color, PIXEL_POS_X(xPos), PIXEL_POS_Y(yPos), SQUARE_SIZE, SQUARE_SIZE);
}

/*
static void reset(int xPos, int yPos){
    if( (xPos % 2 && yPos % 2) || (xPos % 2 == 0 && yPos % 2 == 0)){
        sys_drawRectangle(0xffff00, PIXEL_POS_X(xPos), PIXEL_POS_Y(yPos), SQUARE_SIZE, SQUARE_SIZE);
    }else{
        sys_drawRectangle(0xff00ff, PIXEL_POS_X(xPos), PIXEL_POS_Y(yPos), SQUARE_SIZE,SQUARE_SIZE);
    }
return;
}*/



// last function! --------
 void winner(int * w, struct snakeStruct *s1, struct snakeStruct *s2){
    if(*w){
        sys_playSound(40, 100);
        sys_clearScreen();
        if(flagPlayers==1){
            
            printf("Player 1 points: %d\n",s1->points);
        }else if(*w==2){
            printf("Player 1 won!\n");
            printf("%d vs %d\n", s1->points, s2->points);
        }else{
            printf("Player 2 won!\n");
            printf("%d vs %d\n", s2->points, s1->points);
        }
        printf("Ready for another game? Press y\n");
        char c = getChar();
        if( c == 'y' )
            start();
        else
            exit=1;
    }
}

int welcomeSnake(){
    printf("Welcome to Snake\n");
    printf("Number of players: 1 o 2\n");
    char game = '0';
    while (game != '1' && game != '2') {
        game = getChar();
    }
    return game - CERO; // devuelve en numero en int
}



// TECLADO 


int snakeEnDir(snakeStruct * s, lastMoveEnum move){
    switch(move) {
        case UP: // Up ^
            s->head.y -= 1; 
            if( crash(s)){
                return 1;
            }
            s->lastMove = UP;
            break;
        case DOWN: // Down v
            s->head.y += 1; 
            if( crash(s)){
                return 1;
            }
            s->lastMove = DOWN;
            break;
        case LEFT: // Left <<--
            s->head.x -= 1;
            if( crash(s)){
                return 1;
            }
            s->lastMove = LEFT;
            break;
        case RIGHT: // Right -->>
            s->head.x += 1;
            if( crash(s)){
                return 1;
            }
            s->lastMove = RIGHT;
            break;
        default:
            break; // Movimiento no válido
    }
    if( snakeEatsApple(s)){
        s->points++;
        s->bodyDim++;
        s->body[s->bodyDim-1] = s->head;
        return 0;
    }
    deleteTail(s);
return 0;
}   

int snakeEatsApple(snakeStruct * s){
    if ((s->head.x == apple->cord.x) && (s->head.y == apple->cord.y)) {
        sys_playSound(494,100);
        makeApple();
        return 1;
    }
    return 0;
}

int crash(snakeStruct *s) {
    if (s->head.x < 0 || s->head.x >= X_SQUARES || s->head.y < 0 || s->head.y >= Y_SQUARES) {
        return 1; // Colisión con la pared
    }
    // Verificar colisión con su propio cuerpo o una manzana
    int bodyCrash = boardMatrix[s->head.x][s->head.y];
    if (bodyCrash != CERO && bodyCrash != APPLE) {
        return 1; 
    }

    return 0;
}

void deleteTail(struct snakeStruct * s){
    boardMatrix[s->body[0].x][s->body[0].y] = CERO;
    for(int i=0; i<s->bodyDim-1; i++){
        s->body[i]=s->body[i+1];
    }
    s->body[s->bodyDim-1] = s->head;
}

 int keyPlayerOne(snakeStruct * s){
    int flag =0;
//    char c = 0;
    uint8_t c = 0;
    sys_read(STDIN_FD,&c,1);
    switch (c){
        case KEY_ESC:
            exit = EXIT;
            break;
        case KEY_W:
            if( s->lastMove != DOWN)
                flag = snakeEnDir(s, UP);
            else
                flag=snakeEnDir(s,DOWN);
            break;
        case KEY_S:
            if( s->lastMove != UP)
                flag = snakeEnDir(s, DOWN);
            else
                flag=snakeEnDir(s,UP);
            break;
        case KEY_A:
            if( s->lastMove != RIGHT)
                flag = snakeEnDir(s, LEFT);
            else
                flag=snakeEnDir(s,RIGHT);
            break;
        case KEY_D:
            if( s->lastMove != LEFT)
                flag = snakeEnDir(s, RIGHT);
            else
                flag=snakeEnDir(s,LEFT);
            break;
        default:
            flag = snakeEnDir(s,s->lastMove);
            break;
    }
    return flag;

    }   

 int keyPlayerTwo(snakeStruct *s1, snakeStruct *s2){
    int flagS1 = 0, aux = 0;
//    char c = 0;
    uint8_t c = 0;
    sys_read(STDIN_FD,&c,1);
    switch (c) {
        case KEY_ESC: // Quit game (ESC)
            exit = 1;
            break;
        case KEY_W: // Move up for player 1
            if (s1->lastMove != DOWN) {
                flagS1 = snakeEnDir(s1, UP);
            }else{
                flagS1 = snakeEnDir(s1, DOWN);
            }
            aux = snakeEnDir(s2, s2->lastMove)*2;
            flagS1 = (aux > flagS1) ? aux : flagS1;
            break;
        case KEY_S: // Move down for player 1
            if (s1->lastMove != UP) {
                flagS1 = snakeEnDir(s1, DOWN);
            }else{
                flagS1 = snakeEnDir(s1, UP);
            }
            aux = snakeEnDir(s2, s2->lastMove)*2;
            flagS1 = (aux > flagS1) ? aux : flagS1;
            break;
        case KEY_A: // Move left for player 1
            if (s1->lastMove != RIGHT) {
                flagS1 = snakeEnDir(s1, LEFT);
            }else{
                flagS1 = snakeEnDir(s1, RIGHT);
            }
            aux = snakeEnDir(s2, s2->lastMove)*2;
            flagS1 = (aux > flagS1) ? aux : flagS1;
            break;
        case KEY_D: // Move right for player 1
            if (s1->lastMove != LEFT) {
                flagS1 = snakeEnDir(s1, RIGHT); 
            }else{
                flagS1 = snakeEnDir(s1, LEFT);
            }
            aux = snakeEnDir(s2, s2->lastMove)*2;
            flagS1 = (aux > flagS1) ? aux : flagS1;
            break;
        case KEY_I: // Move up for player 2
            if (s2->lastMove != DOWN) {
                flagS1 = snakeEnDir(s2, UP)*2;
            }else{
                flagS1 = snakeEnDir(s2, DOWN)*2;
            }
            aux = snakeEnDir(s1, s1->lastMove);
            flagS1 = (aux > flagS1) ? aux : flagS1;
            break;
        case KEY_K: // Move down for player 2
            if (s2->lastMove != UP) {
                flagS1 = snakeEnDir(s2, DOWN)*2;
            }else{
                flagS1 = snakeEnDir(s2, UP)*2;
            }
            aux = snakeEnDir(s1, s1->lastMove);
            flagS1 = (aux > flagS1) ? aux : flagS1;
            break;
        case KEY_J: // Move left for player 2
            if (s2->lastMove != RIGHT) {
                flagS1 = snakeEnDir(s2, LEFT)*2;
            }else{
                flagS1 = snakeEnDir(s2, RIGHT)*2;
            }
            aux = snakeEnDir(s1, s1->lastMove);
            flagS1 = (aux > flagS1) ? aux : flagS1;
            break;
        case KEY_L: // Move right for player 2
            if (s2->lastMove != LEFT) {
                flagS1 = snakeEnDir(s2, RIGHT)*2;
            }else{
                flagS1 = snakeEnDir(s2, LEFT)*2;
            }
            aux = snakeEnDir(s1, s1->lastMove);
            flagS1 = (aux > flagS1) ? aux : flagS1;
            break;
        default: // Default movement
            aux = snakeEnDir(s1, s1->lastMove);
            flagS1 = snakeEnDir(s2, s2->lastMove)*2;
            flagS1 = (aux > flagS1) ? aux : flagS1;
            break;
    }
    return flagS1;
}