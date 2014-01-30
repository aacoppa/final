#include <stdlib.h>
#include <stdio.h>
#include "model.h"
#include "map.h"

// (0, 41)
// {NAME, OWNER, CONTINENT, NEIGHBORS, UNITS, X, Y}
territory *territories() {
  territory *ret = calloc(43, sizeof(territory));
  ret[0] = (territory){"Greenland", 
  0, RISK_N_AM, {2, 5, 13, -1}, 1, 351, 120};
  int *nbors;
  ret[1] = (territory){"Alaska",
  0, RISK_N_AM, {2, 3, 31, -1}, 1, 61, 200};
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
                       // SAM
  ret[9] = (territory){"Venezuela",
  0, RISK_S_AM, {8, 10, 11, -1}, 1, 270, 380};
  ret[10] = (territory){"Brazil",
  0, RISK_S_AM, {9, 11, 12, 20, -1}, 1, 330, 440};
  ret[11] = (territory){"Peru",
  0, RISK_S_AM, {9, 10, 12, -1}, 1, 260, 450};
  ret[12] = (territory){"Argentina",
  0, RISK_S_AM, {10, 11, -1}, 1, 280, 530};

  //EUR
  ret[13] = (territory){"Iceland",
  0, RISK_EURO, {0, 14, 15, -1}, 1, 409, 213};
  ret[14] = (territory){"Great Britain",
  0, RISK_EURO, {13, 15, 17, 19, -1}, 1, 442, 252};
  ret[15] = (territory){"Scandinavia",
  0, RISK_EURO, {13, 14, 16, 17, -1}, 1, 504, 195};
  ret[16] = (territory){"Ukraine",
  0, RISK_EURO, {15, 17, 18, 26, 27, 28, -1}, 1, 589, 237};
  ret[17] = (territory){"N. Europe",
  0, RISK_EURO, {14, 15, 18, 19, 16, -1}, 1, 494, 261};
  ret[18] = (territory){"S. Europe",
  0, RISK_EURO, {16, 17, 19, 21, 26, -1}, 1, 511, 294};
  ret[19] = (territory){"W. Europe",
  0, RISK_EURO, {14, 17, 18, 20, -1}, 1, 451, 292};

  //AFRICA
  ret[20] = (territory){"N. Africa",
  0, RISK_AFRICA, {10, 19, 21, 22, 23, -1}, 1, 452, 362};
  ret[21] = (territory){"Egypt",
  0, RISK_AFRICA, {18, 20, 22, 26, -1}, 1, 507, 335};
  ret[22] = (territory){"E. Africa",
  0, RISK_AFRICA, {20, 21, 23, 24, 25, 26, -1}, 1, 556, 387};
  ret[23] = (territory){"Congo",
  0, RISK_AFRICA, {20, 22, 24, -1}, 1, 508, 405};
  ret[24] = (territory){"S. Africa",
  0, RISK_AFRICA, {22, 23, 25, -1}, 1, 518, 462};
  ret[25] = (territory){"Madagascar",
  0, RISK_AFRICA, {22, 24, -1}, 1, 577, 459};


  //ASIA
  ret[26] = (territory){"Middle East",
  0, RISK_ASIA, {16, 18, 21, 22, 27, 37, -1}, 1, 569, 334};
  ret[27] = (territory){"Afghanistan",
  0, RISK_ASIA, {16, 26, 28, 35, 37, -1}, 1, 627, 284};
  ret[28] = (territory){"Ural",
  0, RISK_ASIA, {16, 27, 29, 35, -1}, 1, 658, 222};
  ret[29] = (territory){"Siberia",
  0, RISK_ASIA, {28, 30, 32, 33, 35-1}, 1, 704, 184};
  ret[30] = (territory){"Yakutsk",
  0, RISK_ASIA, {29, 31, 32, -1}, 1, 790, 177};
  ret[31] = (territory){"Kamchatka",
  0, RISK_ASIA, {1, 30, 32, 33, 34, -1}, 1, 872, 186};
  ret[32] = (territory){"Irkutsk",
  0, RISK_ASIA, {29, 30, 31, 33, -1}, 1, 766, 247};
  ret[33] = (territory){"Mongolia",
  0, RISK_ASIA, {29, 31, 32, 34, 35, -1}, 1, 776, 290};
  ret[34] = (territory){"Japan",
  0, RISK_ASIA, {31, 33, -1}, 1, 826, 307};
  ret[35] = (territory){"China",
  0, RISK_ASIA, {27, 28, 29, 33, 36, 37, -1}, 1, 732, 319};
  ret[36] = (territory){"Siam",
  0, RISK_ASIA, {35, 37, 38, -1}, 1, 721, 369};
  ret[37] = (territory){"India",
  0, RISK_ASIA, {26, 27, 35, 36, -1}, 1, 655, 359};

  // AUS
  ret[38] = (territory){"Indonesia",
  0, RISK_AUS, {36, 39, 40, -1}, 1, 730, 424};
  ret[39] = (territory){"New Guinea",
  0, RISK_AUS, {38, 40, 41, -1}, 1, 825, 413};
  ret[40] = (territory){"W. Australia",
  0, RISK_AUS, {38, 39, 41, -1}, 1, 772, 486};
  ret[41] = (territory){"E. Australia",
  0, RISK_AUS, {39, 40, -1}, 1, 847, 479};
  ret[42] = (territory){0};

  return ret;
}

