#include <ncurses.h>            /* ncurses.h includes stdio.h */  
#include <string.h> 
#include <unistd.h>
#include <stdlib.h>
//***Struct***
typedef struct Wall{
    int currentCol; 
    int startRow;
} Wall;
//***Headers***
void printScreen();
void printBars();
void printWall(int wallIndex);
int endGame();
int kbhit();
void checkKeyPress();
void tick();
void makeWall();
void tickActions();
//****Variables***
int totalRow, totalCol;
char * stars;
Wall * walls[15];

//***Game Variables***
int GAME_SPEED = 100000;
int TICKS = 1;

int main(){
    initscr();
    cbreak();
    curs_set(0);
    nodelay(stdscr, TRUE);
    noecho();
    getmaxyx(stdscr,totalRow,totalCol);
    stars = calloc(sizeof(char),totalCol);
    int i = 0;
    while (i < totalCol){
        stars[i] = '*';
        i++;
    }
    Wall firstWall = {totalCol-1, 4};
    walls[0] = &firstWall;
    //Wall secWall = {totalCol-5, 4};
    //walls[1] = &secWall;
    while(1)
        tick();
    while (1){
        checkKeyPress();
    }
    //getch();
    //endwin();

    return 0;
}

void tick(){
    tickActions();
    printScreen();
    usleep(GAME_SPEED);
    TICKS++;
}

void printScreen(){
    clear();
    printBars();
    int i;
    for(i = 0; i < 5; i++){
        if (walls[i])
            //mvprintw(0, 31+i, "%s,%s", walls[i]->currentCol, walls[i]->startRow);
            printWall(i);
    }
    refresh();
}

void tickActions(){
    if (TICKS % 100 == 0)
        GAME_SPEED -= 10000;
    if (TICKS % 10 == 0){
        makeWall();
    }
}

void makeWall(){
    Wall newWall = {totalCol-1, 4};
    int i = 1;
    // while (walls[i])
    //     i++;
    walls[i] = &newWall;
}

void printWall(int wallIndex){
    int i;
    int row = walls[wallIndex]->startRow;
    int col = walls[wallIndex]->currentCol;
    for(i = 0; i < 10; i++){
        mvprintw(row, col, "|");
        row++;
    }
    mvprintw(row, col, "%i", wallIndex);
    walls[wallIndex]->currentCol--;
    if (walls[wallIndex]->currentCol == 1)
        walls[wallIndex]->currentCol = totalCol-1;
}

void printBars(){
    clear();
    mvprintw(0, 0,"%s", stars);
    mvprintw(30,0, "%s", stars);
}

int endGame(){

    return 0;
}

void checkKeyPress(){
    if (kbhit()){
            getch();
            mvprintw(15,totalCol/2, "hi");
            refresh();
            //break;
        }
        else{
            printw("No key pressed yet...\n");
            refresh();
            sleep(1);
        }
}

int kbhit(void){
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}
