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
