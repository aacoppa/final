#include <stdlib.h>
#include <stdio.h>
#include "model.h"
#include "map.h"


territory *territories() {
  territory *ret = calloc(50, sizeof(territory));
  ret[0] = (territory){"Greenland", 
		       0, RISK_N_AM, {2, 5, -1}, 1, 351, 120};
  int *nbors;
  ret[1] = (territory){"Alaska",
                       0, RISK_N_AM, {2, 3, -1}, 1, 61, 200};
  ret[2] = (territory){"North-West Territory",
                       0, RISK_N_AM, {0, 1, 3, 4, -1}, 1, 158, 205};
  ret[3] = (territory){"Alberta",
                       0, RISK_N_AM, {1, 2, 4, 6, -1}, 1, 154, 257};
  ret[4] = (territory){"Ontario",
                       0, RISK_N_AM, {2, 3, 5, 7, 6, -1}, 1, 215, 259};
  ret[5] = (territory){"Quebec",
                       0, RISK_N_AM, {0, 4, 7, -1}, 1, 273, 256};
  ret[6] = (territory){"US West",
                       0, RISK_N_AM, {3, 4, 7, 8, -1}, 1, 162, 307};
  ret[7] = (territory){"US East",
                       0, RISK_N_AM, {4, 5, 6, 8, -1}, 1, 226, 315};
  ret[8] = (territory){"Central America",
                       0, RISK_N_AM, {6, 7, -1}, 1, 193, 361};

  ret[9] = (territory){"Venezuela",
                       0, RISK_S_AM, {8, 10, 11, -1}, 1, 270, 380};
  ret[10] = (territory){"Brazil",
			0, RISK_S_AM, {9, 11, 12, -1}, 1, 330, 440};
  ret[11] = (territory){"Peru",
			0, RISK_S_AM, {9, 10, 12, -1}, 1, 260, 450};
  ret[12] = (territory){"Argentina",
			0, RISK_S_AM, {10, 11, -1}, 1, 280, 530};

  

#warning Other continent data required
  return ret;
}



