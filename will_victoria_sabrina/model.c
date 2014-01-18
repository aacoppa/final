#include "model.h"
#include <string.h>

territory terrs[50]; // need number of territories, should be NULL terminated

net_move rtonetmove(RISK_move m) {
  net_move ret = {m.units, m.origin.name, m.desitnation.name};
  return ret;
}

RISK_move nettormove(net_move m) {
  RISK_move ret = {
    m.units, 
    terr_lookup(m.origin.name),
    terr_lookup(m.destination.name)
  };
  return ret;
}

territory *terr_lookup(char *name) {
  int i = 0;
  for (; terrs[i]; i++) {
    if (strcomp(name, terrs[i].name) == 0)
      return &terrs[i]
  }
  return NULL;
}
