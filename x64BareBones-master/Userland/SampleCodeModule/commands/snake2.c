#include <snake.h>
#include <commands.h>
#include <syscalls.h>
#include <library.h>


// defines 
#define MAX 30
#define PLAYER_2 2
#define CERO '0'


#define P1_ID '#'
#define P2_ID '@'
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
static int exit=0;


static appleStruct * apple;


// chequear chiche 
static void reset(int xPos, int yPos);

void snake2(){
    // sys_clearScreen();
    start(); 
    sys_clearScreen(); // finish = 1 y sale de start();
    return;
}

#define P1_DIM 1
#define P2_DIM 2
#define EXIT 1


void start(){
    exit = 0; // cada vez que arranco se reinicia 
    sys_clearScreen();
    flagPlayers = welcomeSnake();

    printf("\t\t\t\tSnake game starts in 2 seconds\n");
    sys_sleep(2000);
    sys_clearScreen();
    
    board();
    snakeStruct snake1,snake2; 

    spawnSnake(&snake1,P1_DIM); // hasta aca funciona bien
    if( flagPlayers == 2){
        spawnSnake(&snake2, P2_DIM);
    }

    makeApple();
    putAppleInBoard();
    printSnakeInBoard(&snake1, &snake2);


    sys_write(1,"--------------------------",27,0x00ffd7);
    sys_sleep(1000);

    
    refreshSnakeInBoard(&snake1);
    refreshSnakeInBoard(&snake2);
    printAppleInBoard();

    sys_clearScreen();

int w1=0,w2=0;
sys_write(1,"lets goooo",11,0x00ffd7);
    while (exit != EXIT){
        if( flagPlayers == 1){
            w1 = keyPlayerOne(&snake1); 
        }
        if( flagPlayers == 2){
            w2 = keyPlayerTwo(&snake1,&snake2); 
        }   
        refreshSnakeInBoard(&snake1);
        refreshSnakeInBoard(&snake2);
        printSnakeInBoard(&snake1, &snake2);
        winner(&w1,&w2,&snake1, &snake2);
    
    
    }   

}

void board(){
    for( int i=0; i< X_SQUARES; i++){
        for( int j=0; j< Y_SQUARES; j++){
            boardMatrix[i][j] = CERO;
        }
    }
return;
}

// snake Funcs 
void spawnSnake(snakeStruct * s, int playerDim){
    if( playerDim == P1_DIM){
        s->id = P1_ID;
        s->color.colorAscii = 0x00ff00;
        s->head.x = MIN_SNAKE_LENGTH - 1;
        s->head.y = Y_SQUARES/2 +1;
        for(int i = 0; i <= s->head.x; i++){ // Tail (x,y) is the first element of "body" array.
            s->body[i].x = i;
            s->body[i].y = s->head.y;
            boardMatrix[s->head.y][i] = s->id;
        }
        s->bodyDim = MIN_SNAKE_LENGTH;
        s->lastMove = RIGHT;
        s->points = 0;
        boardMatrix[s->head.y][s->head.x] = s->id;
        return;
    }
    // p2 snake
    if( playerDim == 2){
        s->id = P2_ID;
        s->color.colorAscii = 0x00afff;
        s->head.x = X_SQUARES-MIN_SNAKE_LENGTH;
        s->head.y = Y_SQUARES/2 - 1;
        for(int i = X_SQUARES-1, j=0; i >= s->head.x && j < MIN_SNAKE_LENGTH;j++, i--){ // Tail (x,y) is the first element of "body" array.
            s->body[i].x = i;
            s->body[i].y = s->head.y;
            boardMatrix[s->head.y][i] = s->id;
        }
        s->bodyDim = MIN_SNAKE_LENGTH;
        s->lastMove = LEFT;
        s->points = 0;
        boardMatrix[s->head.y][s->head.x] = s->id;
        return;
    }
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
    apple->color.colorAscii = 0xaf0000; // APPLE RED COLOR
    apple->id = APPLE;

    direcs direc;
    direc.x = ownRand(0,X_SQUARES-1);  // insert number
    direc.y = ownRand(1,Y_SQUARES-1);
    while( boardMatrix[direc.x][direc.y] != CERO){
        direc.x = ownRand(0,X_SQUARES-1); // insert number
        direc.y = ownRand(1,Y_SQUARES-1); // chequear
    }
    
    apple->cord.x = direc.x;
    apple->cord.y = direc.y;
    
}

void printAppleInBoard(){
    sys_drawRectangle(apple->color.colorAscii, PIXEL_POS_X(apple->cord.x), PIXEL_POS_Y(apple->cord.y), SQUARE_SIZE, SQUARE_SIZE);
}


void putAppleInBoard(){
    boardMatrix[apple->cord.y][apple->cord.x] = APPLE;

}
// ---- end apple functs


 void printSnakeInBoard(snakeStruct * s1, snakeStruct * s2){
    for(int i=0; i < Y_SQUARES; i++){
        for( int j=0; X_SQUARES; j++){
            if( boardMatrix[i][j] == P1_ID){
                sys_drawRectangle(s1->color.colorAscii, PIXEL_POS_X(j), PIXEL_POS_Y(i), SQUARE_SIZE, SQUARE_SIZE);
                
            }
            else if( boardMatrix[i][j] == P2_ID){
                    sys_drawRectangle(s2->color.colorAscii, PIXEL_POS_X(j), PIXEL_POS_Y(i), SQUARE_SIZE, SQUARE_SIZE);
            }
            else{
                reset(j,i);
            }
           // else if( boardMatrix[i][j] == APPLE){
             //   printAppleInBoard();
        }
           // }else{
            //    sys_write(1,"bocaaaaa\n",10,0x00ffd7);
           //     reset(j,i);
           // }
        }
    
    //sys_write(1,"printSnake\n",12,0x00ffd7);
}

// chequear chiche

static void reset(int xPos, int yPos){
    if( (xPos % 2 && yPos % 2) || (xPos % 2 == 0 && yPos % 2 == 0)){
        sys_drawRectangle(0xffff00, PIXEL_POS_X(xPos), PIXEL_POS_Y(yPos), SQUARE_SIZE, SQUARE_SIZE);
    }else{
        sys_drawRectangle(0xff00ff, PIXEL_POS_X(xPos), PIXEL_POS_Y(yPos), SQUARE_SIZE,SQUARE_SIZE);
    }
return;
}



// last function! --------
 void winner(int * w1, int *w2, struct snakeStruct *s1, struct snakeStruct *s2){
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
        exit=1;
return;

}

int welcomeSnake(){
    printf("Welcome to Snake\n");
    printf("Number of players: 1 o 2\n");
    char game;
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
            s->lastMove = LEFT;
            break;
        default:
            break; // Movimiento no válido
    }
    if( snakeEatsApple(s)){
        s->points++;
        s->bodyDim++;
        s->body[s->bodyDim-1] = s->head;
        putAppleInBoard();
        return 0;
    }
    deleteTail(s);
return 0;
}   

int snakeEatsApple(snakeStruct * s){
    if (s->head.x == apple->cord.x && s->head.y == apple->cord.y) {
        sys_playSound(50,100);
        makeApple();
        return 1;
    }
    return 0;
}

int crash(snakeStruct * s){
    if(boardMatrix[s->head.y][s->head.x] != CERO && boardMatrix[s->head.y][s->head.x] != APPLE ){ // Collision with its snake
        sys_playSound(40,100);
        return 1;
    }
    else if(s->head.x < 0 || s->head.x > X_SQUARES - 1 || s->head.y < 0 || s->head.y > Y_SQUARES - 1){ // Collision with the wall
        return 1;
    }
    return 0;
}

void deleteTail(struct snakeStruct * s){
    boardMatrix[s->body[0].x][s->body[0].y] = CERO;
    int i=1;
    while( i < s->bodyDim){
        s->body[i - 1] = s->body[i];
        i++;
    }
    s->body[i - 1] = s->head;;
}

int checkChar(char c){
    return (c >= 'a' && c <= 'z');
}

 int keyPlayerOne(snakeStruct * s){
    int flag =0;
    while(exit != EXIT){
    char c = getChar();
    if( checkChar(c)){
            switch (c){
                case KEY_ESC:
                    exit = EXIT;
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
    return flag;

    }   
}


 int keyPlayerTwo(snakeStruct *s1, snakeStruct *s2){
    int flagS1 = 0, aux = 0;
    char c = getChar();
    if (checkChar(c)) {
        switch (c) {
            case KEY_ESC: // Quit game (ESC)
                exit = 1;
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
                if (s1->lastMove != RIGHT && s1->lastMove != LEFT) {
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
                if (s2->lastMove != UP && s2->lastMove != DOWN) {
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