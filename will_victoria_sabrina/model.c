#include <stdlib.h>
#include <stdio.h>
#include "model.h"
#include <string.h>

territory *terr_lookup(char *);

net_move rtonetmove(RISK_move m) {
  net_move ret = {m.units, m.origin->name, 
		  m.destination->name};
  return ret;
}

RISK_move nettormove(net_move m) {
  RISK_move ret = {
    m.units, 
    terr_lookup(m.origin),
    terr_lookup(m.destination)
  };
  return ret;
}

territory *terr_lookup(char *name) {
  int i = 0;
  int max = sizeof(terrs)/sizeof(territory);
  for(; i < max; i++) {
    if (strcmp(name, terrs[i].name) == 0)
      return &terrs[i];
    i++;
  }
  return NULL;
}

void log_terr(territory t) {
  printf("%s:\n\tOwner: p%d\n\tUnits: %d\n\tNeighbors:\n", 
	 t.name, t.owner, t.units);
  int i = 0;
  while (t.neighbors[i] > -1) {
    printf("\t\t%s\n", terrs[t.neighbors[i]].name);
    i++;
  }
}
