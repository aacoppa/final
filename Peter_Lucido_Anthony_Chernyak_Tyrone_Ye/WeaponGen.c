#include "headers.h"

#define MAX_WTYPES 4
#define MAX_PART1 4
#define MAX_PART2 4
#define MAX_PART3 4

typedef struct Weapon{
  int type;
  int part1;
  int part2;
  int part3;
  int lvl;
  int attk;
} Weapon;

int left_to_reseed = 4;
int seed;

char *wType[] = {"Pointer","Knife","Sword","Spear"};
double wTypeBase[] = {0,5,10,15};
char *Part1[] = {"","","Shade's",""};
double Part1x[] = {1,1,1.5,1};
char *Part2[] = {"Null","Evil","","Dull","Broken"};
double Part2x[] = {1,1.5,.5,.25};
char *Part3[] = {"","of Doom","",""};
double Part3x[] = {1,1,1.75,1};

Weapon* generateWep(int currlevel);

int main(){
  srand(time(NULL));
  seed = getpid();
  int x = 10;
  while(x > 0){
      Weapon * zero = generateWep(20);
      printf("Weapon created: \n");
      printf("%s %s %s %s (%d) \n", Part1[zero -> part1], Part2[zero -> part2], wType[zero -> type], Part3[zero -> part3], zero -> lvl);
      printf("Attack: %d \n \n", zero->attk);
      free(zero);
      x--;
  }
  return 0;
}
int rand_lim(int limit) {
  if(left_to_reseed){
    srand(seed);
    left_to_reseed = (int)((double)12 * ( rand()/(double)RAND_MAX));
    seed++;
  }
  else
  left_to_reseed--;
  return (int)((double)limit * ( rand()/(double)RAND_MAX));
}
Weapon* generateWep(int currlevel){
  Weapon *blank = (Weapon *) calloc(1,sizeof(Weapon));
  blank -> type = rand_lim(MAX_WTYPES);
  blank -> part1 = rand_lim(MAX_PART1);
  blank -> part2 = rand_lim(MAX_PART2);
  blank -> part3 = rand_lim(MAX_PART3);
  blank -> lvl = rand_lim(currlevel) - rand_lim(currlevel/4) + currlevel/4;
  blank -> attk = (int) (wTypeBase[blank->type] * Part1x[blank->part1]*Part2x[blank->part2]*Part3x[blank->part3]) * blank->lvl/( Part1x[blank->part1]+Part2x[blank->part2]+Part3x[blank->part3]);
  return blank;
}
