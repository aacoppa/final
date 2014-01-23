DW APPROVED

# RISK

##Group
- Will Field-Thompson
- Victoria Greene
- Sabrina Khan

##Concept
Networked RISK game, playable between two or more computers. A world,
made up of continents, in turn composed of territories, must be
conquered, one territory at a time. Each turn a player gets
reinforcements, based on the number of territories in that player's
possesion, with bonuses for continents controlled and cards turned
in. Basically, RISK.

##UI
~~Text based in the terminal, unless there is time to figure out a
graphical display/we are allowed to use another language to write the
interface.~~
SDL2 used to create a graphical interface. A map will be displayed,
with a label displaying number of units on each territory.

##Tech Design
###Specific technologies:
~~- ncurses to deal with displaying menus, etc.~~
~~- there will be a server that handles game state, game logic, etc.~~
- each move will be sent from the client whose turn it is to the
  server, which will distribute it to the rest of the clients
- SDL2 used to display game screen

###Server details 
- the server will recieve communications from a client on each computer
- each move will send data from the client to the server, which will
  handle turns and ensuring that each client gets the current game
  state
- server handles one game at a time

##Timeline
TBD:
1. game logic
2. client
3. server
