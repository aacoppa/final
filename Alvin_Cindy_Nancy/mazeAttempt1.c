#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


typedef struct cell{
  int xpos, ypos;
  int walls[4];//walls = {north, east, south, west}
  int prev[2];// prev = {previous cell's xcor, previous cell's ycor}
  int next[2];// next = {next cell's xcor, next cell's ycor}
  int visited;// 0: no 1: yes
} cell;


//generates a maze with x rows y columns
//each cell is given an xpos(or xcor) and ypos(or ycor) 
//all four walls are up for each cell 
//none of the cells have been visited
void generate(int x, int y, cell grid[y][x]){
  int ycor, xcor;
  for(ycor = 0; ycor < y; ycor++){
    for(xcor = 0; xcor < x; xcor++){
      grid[ycor][xcor].xpos = xcor;
      grid[ycor][xcor].ypos = ycor;
      grid[ycor][xcor].walls[0] = 1 ;
      grid[ycor][xcor].walls[1] = 1 ;
      grid[ycor][xcor].walls[2] = 1 ;
      grid[ycor][xcor].walls[3] = 1 ;
      grid[ycor][xcor].visited = 0;
    }
  }  
}


void printMaze(int y, int x,  cell grid[y][x]){
  int ycor, xcor, counter;
  
  counter = 0;
  for(ycor = 0; ycor < 10; ycor++){
    for(xcor = 0; xcor < 10; xcor++){
      printf("[%d%d,%d%d%d%d]", 
	     grid[ycor][xcor].xpos,
	     grid[ycor][xcor].ypos, 
	     grid[ycor][xcor].walls[0],
	     grid[ycor][xcor].walls[1],
	     grid[ycor][xcor].walls[2],
	     grid[ycor][xcor].walls[3]);
     
      counter++;
      if(counter == 10){
	printf("\n");
	counter = 0;	
      }
    }
  }
}



//breaks down a random wall that is still up and returns cell after it has been changed. 

cell infectCell(int x, int y, cell grid[y][x], cell c){
  
  //array that keeps track of which walls are still up
  int ref[4];
  int i = 0;
  int j = 0;
  while(i < 4){   
    if(c.walls[i] == 1){
	ref[j] == i;
	j++;
    }
    i++;
  }
  
 
  
 
  //counts how many walls are still up
  i = 0;
  int counter = 0;
  while(i < 4){
    if(ref[i] != 0)
      counter++;
    i++;
  }
  
  //if no walls, then return the previous cell
  if(counter == 0){
    return grid[c.prev[1]][c.prev[0]];
  }
  else{
    
    //generates a random number between 0 and (number of walls still up -1)
    srand(time(NULL));
    int rand_wall = (int)(random()%counter);
    
    //takes down a wall that is up
    if((c.xpos != 0 && rand_wall != 3) || (c.xpos != 9 && rand_wall != 1) || (c.ypos != 0 && rand_wall != 0) || (c.ypos != 9 && rand_wall != 2))
      c.walls[rand_wall] = 0;
    else infectCell(x,y, grid, c);
    
    //if number generated is 0, then north wall is taken down. Then keep track of the coordinates of next cell you will visit. 
    if (rand_wall == 0){
      c.next[0] = c.xpos;
      c.next[1] = c.ypos -1;
      grid[c.ypos-1][c.xpos].walls[2] = 0;
    }
     
    //if number generated is 1, then you take down east wall. Then keep track of the coordinates of next cell you will visit. 
    else if(rand_wall == 1){
      c.next[0] = c.xpos+1;
      c.next[1] = c.ypos;
      grid[c.ypos][c.xpos + 1].walls[3] = 0;
    }
     
    //if number generated is 2, then you take down south wall. Then keep track of the coordinates of next cell you will visit. 
    else if(rand_wall == 2){
      c.next[0] = c.xpos;
      c.next[1] = c.ypos +1;
      grid[c.ypos+1][c.xpos].walls[0] = 0;
    }
    
    //take down west wall. Then keep track of the coordinates of next cell you will visit.
    else{
      c.next[0] = c.xpos -1;
      c.next[1] = c.ypos;
      grid[c.ypos][c.xpos -1].walls[1] = 0;
    }
    
    //indicates that this cell has been visited and changed
    c.visited = 1;
    
    //returns cell after the change 
    return c;
  }
}


//randomly picks one cell at beginning and breaks down a random wall 
cell firstCell( int x, int y, cell grid[y][x]){
  srand(time(NULL));
  int rand_xcor = (int)(random()%10);
  int rand_ycor = (int)(random()%10);
  
  printf("x:%d,y:%d\n", rand_xcor, rand_ycor);
  grid[rand_ycor][rand_xcor] = infectCell(x, y, grid, grid[rand_ycor][rand_xcor]);

  return grid[rand_ycor][rand_xcor];
}

/* travels around the grid and breaks down walls, but does not work correctly
void infectNeighbors(int x, int y, cell grid[y][x]){
  cell first = firstCell(x, y, grid);
  cell n = grid[first.next[1]][first.next[0]];
  int alreadyVisited = 1;
  while(alreadyVisited < 101){
    grid[n.ypos][n.xpos] = infectCell(x, y, grid, grid[n.ypos][n.xpos]);
    n = grid[n.next[1]][n.next[0]];
    alreadyVisited++;
  }
  
  }*/
int main(){
  
  cell grid[10][10];
  generate(10, 10,  grid);
  printMaze(10,10, grid);
  
  printf("first cell- random\n");
  // infectNeighbors(10, 10, grid);
  firstCell(10,10, grid);
  printMaze(10, 10, grid);
 
  printf("infecting cell 10\n");
  grid[0][1] = infectCell(10, 10, grid, grid[0][1]);
  printMaze(10, 10, grid);
  return 0;
}
