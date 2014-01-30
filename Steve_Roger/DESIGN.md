DW APPROVED
Make sure you implement some of your pipe/shared memory/netowrk ideas listed below

Text Based Adventure Game
=========================

### Roger Li & Steve Zhu

#### Description:
This will be a text based adventure game played using the terminal.

#### Interface:
The game will be played by selecting numbered choices that are given using commands unique to the game. There will also be other commands used to access other information in the game.

#### Story:
- The antagonist wants to delete the whole system using "rm -rf"
- He realizes that he needs the sudo password
- The protagonist (you) has the password and the antagonist is trying to capture you to get it
- While he is trying to brute force the encrypted key, you go on your journey and defeat him

#### Possible ideas and techniques:
- Allocating memory: Everything in C regarding the use of large amounts of data basically requires memory allocation
- Working with files/Processes: A directory structure representing the areas discovered which can be moved through using "cd" which may possibly be used as a save system. File names/contents may also be used to describe the specifics.
- Signals: Ctrl+C at the very least to quit the game
- Pipes: Maybe make it possible to open 2 terminals at once where one terminal is the medieval world and the other is the future world and you can play both at the same time and they effect each other.
- Shared memory: Following off the previous idea, maybe you have certain items to help you on the way that magically appears in certain ways if you acquire them in either world (shared inventory). Eg. You find a sword in the medieval world and a antique store owner hands you the same sword in the future.
- Network: Maybe data will be stored online so that everything a player does in the game is permanent for every other person who plays.
- Other:
    - You can type custom commands to list your items/stats (if there are stats)/other information
    - ASCII drawings to represent certain things in the story.

#### Algorithms
- Maybe a cryptographic hash algorithm for the key
