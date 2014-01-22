#include <stdlib.h>
#include <stdio.h>
#include "model.h"
#include "map.h"


territory *territories() {
  territory *ret = calloc(50, sizeof(territory));
  int *holder = NULL;
  ret[0] = (territory){"Greenland",
                       0, RISK_N_AM, holder, 1, 351, 120};
  ret[1] = (territory){"Alaska",
                       0, RISK_N_AM, holder, 1, 61, 200};
  ret[2] = (territory){"North-West Territory",
                       0, RISK_N_AM, holder, 1, 158, 205};
  ret[3] = (territory){"Alberta",
                       0, RISK_N_AM, holder, 1, 154, 257};
  ret[4] = (territory){"Ontario",
                       0, RISK_N_AM, holder, 1, 215, 259};
  ret[5] = (territory){"Quebec",
                       0, RISK_N_AM, holder, 1, 273, 256};
  ret[6] = (territory){"US West",
                       0, RISK_N_AM, holder, 1, 162, 307};
  ret[7] = (territory){"US East",
                       0, RISK_N_AM, holder, 1, 226, 315};
  ret[8] = (territory){"Central America",
                       0, RISK_N_AM, holder, 1, 193, 361};

  //territory* nbors[2] = {ret+2, ret+5};
  //ret[0].neighbors = nbors;
  /* #warning Alaska connects to Asia */

  /* nbors = {ret+2, ret+3}; */
  /* ret[1].neighbors = nbors; */
  /* ret[2].neighbors = (territory**){ret+0, ret+1, */
  /* 				   ret+3, ret+4}; */
  
    ret[9] = (territory){0};
  return ret;
}



