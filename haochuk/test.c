#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void printMap(int **map);
void move(int **map,int y,int x,char buffer[256]);
int main(){
  int **map = calloc(sizeof(int),11);
  int x,y;
  char buffer[256];
  x = 5;
  y = 5;
  int i = 0;
  int j = 0;
  for(i = 0;i<11;i++)
    map[i] = calloc(sizeof(int),11);
  map[0][0] = 'X';
  for(i = 0;i<10;i++){
    map[0][i+1] = i;
    map[i+1][0] = 'A'+i;
  }
  
  move(map,y,x,buffer);
 
  while(1){
    printMap(map); 
    printf("where do you want to move: \n");
    fgets(buffer,sizeof(buffer),stdin);
    //    move(map,&y,&x,buffer); 
    map[abs(y%11)][abs(x%11)] = 0;
    if (strchr(buffer,'w'))
      y--; 
    else if (strchr(buffer,'s'))
      y++;
    else if (strchr(buffer,'d'))
      x++;
    else if (strchr(buffer,'a'))
      x--;
    map[y][x] = 9;
  }
}

void printMap(int **map){
  int r = 0;
  int c = 0;
  while(r<11){
    while(c<11){
      if(c)
	printf("%i ",map[r][c]);
      else
	printf("%c ",map[r][c]);
      c++;
    }
    printf("\n");
    r++;
    c = 0;
  }
}
   
void move(int **map,int y,int x,char buffer[256]){
  map[y][x] = 0;
  if (strchr(buffer,'w'))
    y--;
  else if (strchr(buffer,'s'))
    y++;
  else if (strchr(buffer,'d'))
    x++;
  else if (strchr(buffer,'a')){
    x--;
  }
  map[y%11][x%11] = 9;
}
