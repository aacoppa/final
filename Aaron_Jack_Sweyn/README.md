RXC
===

A multiplayer racing game!
Race through randomly generataed maps and challenge your friends


INSTRUCTIONS
============

There are two sides of the program to be run.


/server
------
make clean #Optional, removes existing db, clears users and games...
make
./server

This will start running the server; initializing a database, etc

/client
-------
make
./rxc help #Displays the different options available to a client

Suggested actions for a teacher planning on testing the program...

./rxc register NAME1 PASS1
./rxc register NAME2 PASS2 #Logs you in as the second user

./rxc games #Displays that you have no games
./rxc play NAME1 #Starts a game against NAME1

./rxc login NAME1 PASS1
./rxc games #Displays the challenge
./rxc play NAME2 #Respond to the challenge



Concepts
========

We have taken advantage of our learning in threading, locking, server/client communications
and many other topics from Systems. We use a database to store games, but everything else is
done client side.
