
DESIGN DOC
==========

Team
====
Aaron Coppa, Sweyn Venderbush, Jack Cahn

What is it?
===========

We are creating a multiplayer command line game with a turn-based implementation to challenge your friends.

In the game itself, you are a **@** and are racing to the right of the screen, avoiding barriers, reminiscent of the game "Helicopter."

The server setup allows a user to play a set of games against another user, racing on the same identical random level to try to survive the longer distance.

Roles
=====

* ASCII Graphic artist
* Backend multiplayer monger
* Game mechanics and level generation master

Timeline
========

* Create working backend ASAP, by this week's end
* Explore ASCII graphics by end of next weekend
* Start game generation by next Wednesday

User Interface
==============

The server itself has no user interface.  It sits idle unless interacting with the client.  
The client functions via a set of commands.  The only time the client runs for a prolonged period of time is when the actual game is being played.  
A user registers and/or logs into the client, gets the current games s/he is in, and then can choose to continue a current game or start a new one with another registered username.  
**See README for an example set of commands**

Concepts
========

* Server-Client relationship through multiplayer aspect
* Non-static client relationship, where a connection only exists when a command is being run on the client
* Implementation of SQL in C
* A platform independent pseudo-random number generator that allows the game to create infinite, semi-random levels that are replicate as long as the same key is used.
* A weird variety of structs to simulate objects in C
* Terminal windowing and updating using the NCURSES library
* In-depth error catching

