// rolls a six-sided die
int die_roll();

/*
  Simulates a battle between attacker and defender.
  For each unit defending or attacking, a die is rolled. 
  Highest defending roll is compared to highest attacking roll.
  In case of tie, defense wins.
  Continue until one side has 0 units left.
  @param uAtt : units sent by attacker
  @param uDef : units defending

  RETURNS : 0 for attacking win, 1 for defending win
*/
int battle(int uAtt, int uDef);
