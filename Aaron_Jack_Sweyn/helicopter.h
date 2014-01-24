#include <ncurses.h> 
#include <string.h> 
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>
#include <sys/time.h>

//***Structs***
typedef struct Wall{
    int currentCol; 
    int startRow;
    int show;
    int length;
    int color;
} Wall;
typedef struct Player{
    int playerRow;
    int alive;
} Player;

//***Headers***
void printScreen();
void printBars();
void printWall(int wallIndex);
int kbhit();
void seedGame(unsigned long seed);
void checkKeyPress();
void tick();
unsigned long getTime(void);
void makeWall();
void tickActions();
int getRandom(void);
void printPlayer();
int isTick();
int startGame(unsigned long seed);
unsigned long makeSeed();
void deleteWall(int wallIndex);
void initiateRand();

//****Variables***
int totalRow, totalCol;
char * stars;
Wall walls[100];
Player player;

//****Random Variables****
unsigned long next = 1;
unsigned long gameSeed;

//***Game Variables***
int GAME_SPEED = 80;
int TICKS = 1;
unsigned long lastTick;
unsigned long nextDrop = 0;
int betweenWalls = 20;