// Continent enum, each territory will belong to one Continent
typedef enum Continent {
  RISK_ASIA,	// Asia
  RISK_N_AM,	// North America
  RISK_S_AM,	// South America
  RISK_EURO,	// Europe
  RISK_AFRICA,	// Africa
  RISK_AUS	// Australia (Oceania)
} Continent;

// A territory to be held by a given player (owner)
typedef struct territory {
  char *name;				// name
  int owner;				// A player id
  Continent cont;			// Continent it belongs to
  int neighbors[4];			// indices of neighbor territories
  int units;				// units in territory
  int x, y;				// coordinates of circle
} territory;

/* 
   Defines a move made, which can be abstracted as
   movement of some number of units from one territory
   to the next. This structure is intended to be a simple
   update to the server on each player's turn.
   
   CASES:
   origin == destination && units < 0	: territory lost units,
                                          but didn't change hands
   
   origin.owner != destination.owner	: destination lost a battle,
                                          units to be moved

   origin.owner == destination.owner	: owner is moving troops
   
   origin == NULL			: owner is resupplying destination
*/

typedef struct RISK_move {
  int units;
  territory *origin; //pointer to territory
  territory *destination; //pointer to territory
} RISK_move;
  
typedef struct net_move {
  int units;
  char *origin; //name of territory
  char *destination; //name of territory
} net_move;

net_move rtonetmove(RISK_move m);
RISK_move nettormove(net_move m);
void log_terr(territory t);
territory *terrs; // need number of territories, should be NULL terminated
