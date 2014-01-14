// A territory to be held by a given player (owner)
typedef struct territory {
  int owner;				// A player id
  Continent cont;			// Continent it belongs to
  struct territory ** neighbors;	// array of pointers to all neighbors
					// each neighbor will have a link back
} territory;

// Continent enum, each territory will belong to one Continent
typedef enum Continent {
  RISK_ASIA,	// Asia
  RISK_N_AM,	// North America
  RISK_S_AM,	// South America
  RISK_EURO,	// Europe
  RISK_AFRICA,	// Africa
  RISK_AUS	// Australia (Oceania)
} Continent;
