#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

void generatePlayer();
void printStats();
void generateRoom();
void interpretGame();
void battle();
void levelUp();
void printRoomInfo();
void generateEnemy();
int rand_lim(int limit);
void DisplayHelp();
void nameGenerator();
void dump();
int load();
void pickUpLoot();

typedef struct Weapon{
  int type;
  int part1;
  int part2;
  int part3;
  int lvl;
  int attk;
} Weapon;
typedef struct Character{
  char name[256];
  int atk;
  int def;
  int spd;
  int hp;
  int maxHp;
  int xp;
  int level;
  int xpOffset;
  int weaponNum;
  Weapon slot1;
  Weapon slot2;
} Character;
typedef struct Room{
  Character Enemy;
  int room;
  int roomClear;
  int roomXp;
  Weapon loot;
} Room;




Weapon generateWep(int currlevel);
