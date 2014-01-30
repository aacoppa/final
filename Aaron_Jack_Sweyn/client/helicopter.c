#include "helicopter.h"

/*
int main(){
    initiateRand();
    game_start(makeSeed());
} 
*/
void reset_vars() {
    next = 1;
    GAME_SPEED = 80;
    TICKS = 1;
    nextDrop = 0;
    betweenWalls = 20;
}
int game_start(unsigned long seed){
    reset_vars();
    seedGame(seed);
    FILE *file_id = fopen("/dev/tty", "r+"); 
    SCREEN *termref = newterm(NULL, file_id, file_id);
    cbreak();
    curs_set(0);
    start_color();
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
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
    printBars();
    mvprintw(32, 0, "Current Distance:");
    while(player.alive)
        tick();
    clear();
    endwin();
    return TICKS;
}

void tick(){
    if (isTick())
        tickActions();
    usleep(40);
    checkKeyPress();
    usleep(40);
    printScreen();
    usleep(40);
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

int isDrop(){
    return getTime() > nextDrop;
}

unsigned long getTime(void){
    struct timeval  tv;
    gettimeofday(&tv, NULL);
    unsigned long time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    return time_in_mill;
}

void printScreen(){
    mvprintw(32, 18, "%d", TICKS);
    refresh();
}

void tickActions(){
    TICKS++;
    if (TICKS % 100 == 0 && GAME_SPEED > 20){
        GAME_SPEED -= 2;
        if (betweenWalls > 10)
            betweenWalls--;
    }
    if (TICKS % betweenWalls == 0){
        makeWall();
    }
    if (isDrop()){
        if (TICKS % 2 == 0){
            if (player.playerRow < 29){
                mvprintw(player.playerRow, 5, " ");
                player.playerRow++;
                printPlayer();
            }
            else
                player.alive = 0;
        }
    }
    int wallIndex;
    for (wallIndex = 0; wallIndex < 100; wallIndex++){
        walls[wallIndex].currentCol--;
        if (walls[wallIndex].currentCol == 1)
            walls[wallIndex].show = 0;
    }
    int i;
    for(i = 0; i < 100; i++){
        if (walls[i].show == 1)
            printWall(i);
        else
            deleteWall(i);
    }
}

void makeWall(){
    Wall newWall;
    int random = getRandom();
    newWall.length = random % 20 + 1;
    newWall.currentCol = totalCol-1;
    newWall.startRow = random % (30 - newWall.length) + 1;
    newWall.show = 1;
    int c = rand() % 2;
    newWall.color = 2 + c;
    int i = 0;
    while (walls[i].show == 1)
        i++;
    walls[i] = newWall;
}

void printWall(int wallIndex){
    int i;
    int row = walls[wallIndex].startRow;
    int col = walls[wallIndex].currentCol;
    attron(COLOR_PAIR(walls[wallIndex].color));
    for(i = 0; i < walls[wallIndex].length; i++){
        mvprintw(row, col+1, " ");
        mvprintw(row, col, "|");
        if (row == player.playerRow && col == 5)
            player.alive = 0;
        row++;
    }
    attroff(COLOR_PAIR(walls[wallIndex].color));
}

void deleteWall(int wallIndex){
    int i;
    int row = walls[wallIndex].startRow;
    int col = walls[wallIndex].currentCol;
    attron(COLOR_PAIR(walls[wallIndex].color));
    for(i = 0; i < walls[wallIndex].length; i++){
        mvprintw(row, col+1, " ");
        row++;
    }
    attroff(COLOR_PAIR(walls[wallIndex].color));
}

void printPlayer(){
    mvprintw(player.playerRow, 5, "@");
}

void printBars(){
    attron(COLOR_PAIR(1));
    mvprintw(0, 0,"%s", stars);
    mvprintw(30,0, "%s", stars);
    attroff(COLOR_PAIR(1));
}

void checkKeyPress(){
    if (kbhit()){
        int key = getch();
        if (key == 'w'){
            if (player.playerRow > 1){
                mvprintw(player.playerRow, 5, " ");
                player.playerRow--;
                printPlayer();
                nextDrop = getTime() + 250;
            }
            else{
                player.alive = 0;
            }
        }
    }
}

int kbhit(void){
    int ch = getch();
    if (ch != ERR){
        ungetch(ch);
        return 1;
    } 
    else{
        return 0;
    }
}

void initiateRand(){
    srand(time(0));
}

unsigned long makeSeed(){
    return rand();
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
