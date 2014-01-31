#include <stdio.h>
#include <stdlib.h>
#include "model.h"
#include <string.h>
#include <bsd/stdlib.h>

// check if territories are adjacent
char tadjacent(territory *t1, territory *t2) {
  int *p = t1->neighbors;
  while(*p > -1) {
    if (strcmp(terrs[*p].name, t2->name) == 0)
      return 1;
    p++;
  }
  return 0;
}

int die_roll() {
  int roll = (int)(arc4random() % 6 + 1);
  return roll;
}

int battle(int uAtt, int uDef) {
  while (uAtt > 0 && uDef > 0) {
    int aRoll = die_roll();
    int dRoll = die_roll();
    if (aRoll > dRoll) {
      uDef--;
    }
    else {
      uAtt--;
    }
  }
  if (uAtt == 0) {
    return 1;
  }
  return 0;
}

void distribute(int numPlayers) {
  int counter = 0;
  int temp[42];
  while (counter <= 41) {
    *(temp + counter) = counter;
    counter++;
  }
  int curr = 1;
  counter = 0;
  while (counter <= 41) {
    int a = (int)(arc4random() % 42);
    if (*(temp + a) != -1) {
      (*(terrs + *(temp + a))).owner = curr;
      *(temp + a) = -1;
      if (curr == numPlayers) {
        curr = 1;
      }
      else {
        curr++;
      }
      counter++;
    }
  }
}

