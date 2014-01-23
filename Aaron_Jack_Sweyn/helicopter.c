#include "helicopter.h"

int main(){
    startGame(makeSeed());
}

int startGame(unsigned long seed){
    seedGame(seed);
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
    player.alive = 1;
    makeWall();
    while(player.alive)
        tick();
    clear();
    endwin();
    return TICKS;
}

void tick(){
    if (isTick())
        tickActions();
    usleep(30);
    checkKeyPress();
    usleep(30);
    printScreen();
    usleep(20);
}

int isTick(){
    unsigned long currentTime = getTime();
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
    mvprintw(32, 0, "Current Distance: %d", TICKS);
    int i;
    int j = 35;
    for(i = 0; i < 100; i++){
        if (walls[i].show == 1)
            printWall(i);
    }
    printPlayer();
    refresh();
}

void tickActions(){
    TICKS++;
    if (TICKS % 100 == 0 && GAME_SPEED > 20)
        GAME_SPEED -= 1;
    if (TICKS % 10 == 0){
        makeWall();
    }
    if (TICKS % 2 == 0){
        player.playerRow++;
    }
    int wallIndex;
    for (wallIndex = 0; wallIndex < 100; wallIndex++){
        walls[wallIndex].currentCol--;
        if (walls[wallIndex].currentCol == 1)
            walls[wallIndex].show = 0;
    }
}

void makeWall(){
    Wall newWall;
    int random = getRandom();
    newWall.length = random % 10 + 1;
    newWall.currentCol = totalCol-1;
    newWall.startRow = random % (30 - newWall.length) + 1;
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
    for(i = 0; i < walls[wallIndex].length; i++){
        mvprintw(row, col, "|");
        if (row == player.playerRow && col == 5)
            player.alive = 0;
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
            if (player.playerRow > 1)
                player.playerRow--;
        // if (key == 's')
        //     if (player.playerRow < 29)
        //         player.playerRow++;
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

unsigned long makeSeed(){
    srand(time(0));
    return random();
}

/* RAND_MAX assumed to be 32767 */
int getRandom(void){
    next = next * 1103515245 + 12345;
    return((unsigned)(next/65536) % 32768);
}

void seedGame(unsigned long seed){
    gameSeed = seed;
    next = gameSeed;
}
