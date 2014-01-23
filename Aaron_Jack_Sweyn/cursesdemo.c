#include <ncurses.h>            /* ncurses.h includes stdio.h */  
#include <string.h> 
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>
#include <sys/time.h>
//***Struct***
typedef struct Wall{
    int currentCol; 
    int startRow;
    int show;
} Wall;
typedef struct Player{
    int playerRow;
} Player;
//***Headers***
void printScreen();
void printBars();
void printWall(int wallIndex);
int endGame();
int kbhit();
void checkKeyPress();
void tick();
unsigned long getTime(void);
void makeWall();
void tickActions();
void seedGame();
int getRandom(void);
void printPlayer();
int isTick();
//****Variables***
int totalRow, totalCol;
char * stars;
Wall walls[15];
Player player;
//****Random****
unsigned long next = 1;
unsigned long gameSeed;

//***Game Variables***
int GAME_SPEED = 20;
int TICKS = 1;
unsigned long lastTick;

int main(){
    seedGame();
    initscr();
    cbreak();
    curs_set(0);
    nodelay(stdscr, TRUE);
    noecho();
    getmaxyx(stdscr,totalRow,totalCol);
    stars = calloc(sizeof(char),totalCol);
    int i = 0;
    lastTick = getTime();
    while (i < totalCol){
        stars[i] = '*';
        i++;
    }
    player.playerRow = 15/2;
    makeWall();
    while(1)
        tick();
    endwin();

    return 0;
}

void tick(){
    if (isTick())
        tickActions();
    checkKeyPress();
    printScreen();
    usleep(40);
}

int isTick(){
    unsigned long currentTime = getTime();
    // mvprintw(50, 0, "%ld", currentTime);
    // mvprintw(51, 0, "%ld", lastTick);
    // mvprintw(52, 0, "%ld", currentTime - lastTick);
    int ans = 0;
    if (currentTime - lastTick > GAME_SPEED){
        ans = 1;
        lastTick = currentTime;
    }
    return ans;
}

unsigned long getTime(void){
    struct timeval  tv;
    gettimeofday(&tv, NULL);
    unsigned long time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    return time_in_mill;
}

void printScreen(){
    clear();
    printBars();
    int i;
    for(i = 0; i < 15; i++){
        if (walls[i].show == 1)
            printWall(i);
    }
    printPlayer();
    refresh();
}

void tickActions(){
    TICKS++;
    if (TICKS % 100 == 0)
        //GAME_SPEED -= 1000;
    if (TICKS % 50 == 0){
        makeWall();
    }
    if (TICKS % 2 == 0){
        //player.playerRow++;
    }
    int wallIndex;
    for (wallIndex = 0; wallIndex < 15; wallIndex++){
        walls[wallIndex].currentCol--;
        if (walls[wallIndex].currentCol == 1)
            walls[wallIndex].show = 0;
    }
}

void makeWall(){
    Wall newWall;
    newWall.currentCol = totalCol-1;
    newWall.startRow = getRandom() % 19 + 1;
    newWall.show = 1;
    int i = 0;
    while (walls[i].show == 1)
        i++;
    walls[i] = newWall;
}

void printWall(int wallIndex){
    int i;
    int row = walls[wallIndex].startRow;
    int col = walls[wallIndex].currentCol;
    for(i = 0; i < 10; i++){
        mvprintw(row, col, "|");
        row++;
    }
}

void printPlayer(){
    mvprintw(player.playerRow, 5, "@");
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
        int key = getch();
        if (key == 'w')
            player.playerRow--;
        if (key == 's')
            player.playerRow++;
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


/* RAND_MAX assumed to be 32767 */
int getRandom(void){
    next = next * 1103515245 + 12345;
    return((unsigned)(next/65536) % 32768);
}

void seedGame(){
    gameSeed = time(0);
    next = gameSeed;
}
