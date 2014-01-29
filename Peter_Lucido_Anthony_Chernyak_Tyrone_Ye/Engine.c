#include "headers.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_WTYPES 5
#define MAX_PART1 5
#define MAX_PART2 5
#define MAX_PART3 5

char enemyNames[3][256];
int nameSize = 2;

char mainMenu[256];
char user[256];
char input[256];
char statUp[256];

Character * Player;
Character * Enemy;
Room * DRoom;

int left_to_reseed = 4;
int seed;

char *wType[] = {"Pointer","Knife","Sword","Spear","Halberd"};
double wTypeBase[] = {0,5,10,15,20};
char *Part1[] = {"","","Shade's","","Beginner's"};
double Part1x[] = {1,1,1.5,1,.5};
char *Part2[] = {"Null","Evil","","Dull","Broken"};
double Part2x[] = {1,1.5,.5,.25};
char *Part3[] = {"","of Doom","","","of insignifigance"};
double Part3x[] = {1,1,1.75,1,.25};

int main(){
  nameGenerator();
  //inventory = (Weapon)calloc(1,sizeof(Weapon));
  srand(time(NULL));
  seed = getpid();
  
  while(1){
    system("clear");
    printf("Welcome to Generic Crawler #%d!\nType Play to start playing, or Exit to Quit the game.\n",rand_lim(545));
    fgets(mainMenu, sizeof(mainMenu), stdin);
    if(strcasecmp("Play\n", mainMenu) == 0){
      /// printf("Game is Go\n");
      printf("Load Game?\n");
      fgets(input,sizeof(input), stdin);
      int bob = 2;
      if(strcasecmp(input,"Yes\n") == 0)
	bob = 1;
      switch(bob){
      case 1:
	if(!LoadChooser()){
	  printf("Test\n");
	  break;
	}
	else 
	  printf("There is no save file present. Creating new Character\n");
      case 2:
	Player = (Character *)calloc(1, sizeof(Character));
	printf("Please enter your Name:\n");
	fgets(user,sizeof(user),stdin);
	strcpy(Player->name,user);
	printf("Generating Stats\n");
	generatePlayer();
	printStats();
	system("clear");
	printf("Would You Like to see the Help Page?\n");
	while(1){
	  fgets(input,sizeof(input),stdin);
	  if(strcasecmp("Yes\n",input) == 0){
	    DisplayHelp();
	    break;
	  }
	  else if(strcasecmp("No\n",input) == 0){
	    system("clear");
	    break;
	  }
	  else{
	    printf("Sorry. I don't understand what you said\n");
	  }	  
	}
	DRoom = calloc(1,sizeof(Room));
	generateRoom();
      }
      while(Player->hp > 0){
	system("clear");
	printf("Your life: %d \n", Player->hp);
	if(!DRoom->roomClear)
	  printf("There is a %s in this room\n", Enemy->name);
	printf("What would you like to do?\n");
	fgets(input,sizeof(input), stdin);
	interpretGame();
      }
      printf("YOU LOST!\n");
      sleep(2);
    }
    else if(strcasecmp("Exit\n",mainMenu) == 0){
      printf("Goodbye\n");
      break;
    }
    else if(strcasecmp("What What?\n",mainMenu) == 0){
      printf("NO. WE ARE NOT DOING THAT!\n");
      sleep(1);
    }
    else{
      printf("Sorry. I don't understand what you said\n");
      sleep(2);
    }
  }
  return 1;
}

//Utilities-----------------------------------------------------------------------------------------Utilities

/*int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;

    retval = rand();
    while(retval >= limit){
      retval = retval - limit;
    }

    return retval;
    }*/
int rand_lim(int limit) {
  if(left_to_reseed){
    srand(seed);
    left_to_reseed = (int)((double)12 * ( rand()/(double)RAND_MAX));
    seed++;
  }
  else
  left_to_reseed--;
  return (int)((double)limit * ( rand()/(double)RAND_MAX));
}

//Generation-----------------------------------------------------------------------------------------Generation

void generatePlayer(){
  if(!strcmp(Player->name,"Lu Bu\n")){
    Player->atk = 999;
    Player->def = 999;
    Player->spd = 1;
    Player->hp = 999;
    Player->maxHp = Player->hp;
    Player->xp = 0;
    Player->level = 10;
    Player->xpOffset = 999;
    Player->weaponNum = 0;
    Player->slot1 = *(Weapon *)calloc(1,sizeof(Weapon));
    Player->slot2 = *(Weapon *)calloc(1,sizeof(Weapon));
  }
  else{
    Player->atk = rand_lim(12) + 6;
    Player->def = rand_lim(12) + 6;
    Player->spd = rand_lim(12) + 6;
    Player->hp = rand_lim(12) + 6;
    Player->maxHp = Player->hp;
    Player->xp = 0;
    Player->level = 1;
    Player->xpOffset = 10;
    Player->weaponNum = 0;
    Player->slot1 = *(Weapon *)calloc(1,sizeof(Weapon));
    Player->slot2 = *(Weapon *)calloc(1,sizeof(Weapon));
  }
}

void nameGenerator(){
  strcpy(enemyNames[0],"Giant");
  strcpy(enemyNames[1],"Rat");
  strcpy(enemyNames[2],"Skeleton");
}

void generateRoom(){
  int room = DRoom -> room;
  DRoom -> room = DRoom -> room + 1;
  generateEnemy();
  printf("There is a %s in this room\n", Enemy->name);
  DRoom -> roomClear= 0;
  DRoom -> roomXp = rand_lim(Player->level) + Player->level + room;
  generateWep(Player->level);
}

void generateEnemy(){
  int room = DRoom -> room;
  DRoom -> Enemy.atk = rand_lim(10 + room) + Player->level;
  DRoom -> Enemy.def = rand_lim(10 + room) + Player->level;
  DRoom -> Enemy.spd = rand_lim(10 + room) + Player->level;
  DRoom -> Enemy.hp = rand_lim(10 + room) + Player->level;
  int temp = rand_lim(nameSize);
  strcpy(DRoom -> Enemy.name,enemyNames[temp]);
  Enemy = &(DRoom -> Enemy);
  //int temp = rand_lime(2);
  //strcpy(EnemyList[temp],Enemy->name);
}

Weapon generateWep(int currlevel){
  DRoom->loot = *(Weapon *) calloc(1,sizeof(Weapon));
  DRoom->loot.type = rand_lim(MAX_WTYPES);
  DRoom->loot.part1 = rand_lim(MAX_PART1);
  DRoom->loot.part2 = rand_lim(MAX_PART2);
  DRoom->loot.part3 = rand_lim(MAX_PART3);
  DRoom->loot.lvl = rand_lim(currlevel) - rand_lim(currlevel/4) + currlevel/4 + 1;
  DRoom->loot.attk = (int) ((wTypeBase[DRoom->loot.type] * 
			     Part1x[DRoom->loot.part1]*
			     Part2x[DRoom->loot.part2]*
			     Part3x[DRoom->loot.part3]) * 
			     DRoom->loot.lvl/(
					       Part1x[DRoom->loot.part1]+
					       Part2x[DRoom->loot.part2]+
					       Part3x[DRoom->loot.part3]));
  return DRoom->loot;
}

//Print Outputs---------------------------------------------------------------------------------Print Outputs

void printStats(){
  printf("Name: %s\nAttack: %d\nDefense: %d\nSpeed: %d\nHealth: %d\nLevel : %d\nCurrent XP : %d\nXP until until next level: %d\n",Player->name, Player->atk, Player->def, Player->spd,Player->hp,Player->level,Player->xp,(Player->xpOffset - Player->xp));
  printf("\n Press enter to continue \n");
  getchar();
}

void DisplayHelp(){
  printf("\nCOMMANDS AND FUNCTIONS\nStats - Displays your Attack, Defense, Speed, Current XP,\n\t XP until next level, and Health\nRoom - Current Room information\nInventory - Display your current Inventory\nHelp - Displays the Help\nAttack - Attacks the monster if there is one in the room\nAdvance- Moves you to the next room if it is possible\nSave - save the game (note: you may only have one save file)\nOSave - Saves to an online server (You MUST have the server running and correct ip address to do this)\n ");
  printf("\n Press enter to continue \n");
  getchar();
}

void printRoomInfo(){
  printf("This is Room %d\n", DRoom -> room);
  if(!DRoom -> roomClear){
    printf("There is a monster in this room\n");
  }
  else{
    printf("There is no monster in this room\n");
  }
  sleep(2);
}

//Game functions------------------------------------------------------------------------------------Game Functions

void interpretGame(){
  if(strcasecmp(input,"Help\n") == 0){
    system("clear");
    DisplayHelp();
  }
  else if(strcasecmp(input,"Stats\n") == 0){
    system("clear");
    printStats();
  }
  else if(strcasecmp(input,"Room\n") == 0){
    system("clear");
    printRoomInfo();
  }
  else if(strcasecmp(input,"Attack\n") == 0){
    battle();
  }
  else if(strcasecmp(input,"down forward low punch\n") == 0){
    printf("Hadoken \n");
    sleep(1);
    if(DRoom->roomClear)
      printf("A blue orb comes out of your hands and hits the wall\n");
    else
      printf("%s: ... \n", Enemy->name);
    sleep(1);
  }
  else if(strcasecmp(input,"Advance\n") == 0){
    if(!DRoom -> roomClear){
      printf("The room isn't clear yet!\n");
      sleep(1);
    }
    else{
      printf("You move into the next room\n");
      generateRoom();
    }
  }
  else if(!strcasecmp(input,"Save\n")){
    dump();
    printf("Game saved\n");
  }
    else if(!strcasecmp(input,"OSave\n")){
    dump();
    char buff[256];
    printf("Enter the ip adress of the save server\n");
    fgets(buff, sizeof(buff), stdin);
    OSave(2, buff);
  }
  else if(strcasecmp(input,"Inventory\n") == 0){
    if(Player->weaponNum == 0){
	printf("You have no weapons!\n");
	getchar();
    }
    else if(Player->weaponNum == 1){
	printf("Your weapons give you a +%d bonus to attack.\n",Player->slot1.attk);
	getchar();
    }
    else if(Player->weaponNum == 2){
      printf("Your weapons give you a +%d bonus to attack.\n",Player->slot1.attk + Player->slot2.attk);
      getchar();
    }
  }
  else{
    printf("Sorry, I don't understand that\n");
    sleep(1);
    system("clear");
  }
}

void battle(){
  int roomXp = DRoom -> roomXp;
  if(Enemy->hp <= 0){
    printf("There is no monster in this room\n");
    sleep(1);
  }
  else{
    int dmg;
    if(Player->spd >= Enemy->spd){
      dmg = rand_lim(Player->atk) + Player->atk - Enemy->def;
      if(dmg < 0)
	dmg = 0;
      printf("You dealt %d damage to the %s\n", dmg,Enemy->name);
      Enemy->hp = Enemy->hp - dmg;
      sleep(1);
      if(Enemy->hp > 0){
	dmg = rand_lim(Enemy->atk) + Enemy->atk - Player->def;
	if(dmg < 0)
	  dmg = 0;
	printf("The %s dealt %d damage to you\n", Enemy->name, dmg);
	sleep(1);
	Player->hp = Player->hp - dmg;
      }
      else{
	printf("The %s died!\n", Enemy->name);
	sleep(1);
	DRoom -> roomClear = 1;
	pickUpLoot();
	Player->xp = Player->xp + roomXp;
	while(Player->xp >= Player->xpOffset){
	  levelUp();
	}
      }
    }
    else{
      dmg = rand_lim(Enemy->atk) + Enemy->atk - Player->def;
      if(dmg < 0)
	dmg = 0;
      printf("The %s dealt %d damage to you\n", Enemy->name, dmg);
      Player->hp = Player->hp - dmg;
      sleep(1);
      if(Player->hp > 0){
	dmg = rand_lim(Player->atk) + Player->atk - Enemy->def;
	if(dmg < 0)
	  dmg = 0;
	printf("You dealt %d damage to the %s\n", dmg, Enemy->name);
	sleep(1);
	Enemy->hp = Enemy->hp - dmg;
	if(Enemy->hp <= 0){
	  printf("The %s  died!\n", Enemy->name);
	  sleep(1);
	  DRoom -> roomClear = 1;
	  pickUpLoot();
	  Player->xp = Player->xp + roomXp;
	  while(Player->xp >= Player->xpOffset){
	    levelUp();
	  }
	}
      }
    }
  }
}

void levelUp(){
  int skillPoints = 3;
  Player->level = Player->level + 1;
  printf("You have leveled up. You are now level %d\n", Player->level);
  Player->xp = Player->xp - Player->xpOffset;
  Player->xpOffset = (Player->level * 10);
  while(skillPoints > 0){
    printf("Attack: %d \n",  Player->atk);
    printf("Defense: %d \n",Player->def);
    printf("Speed: %d \n",Player->spd);
    printf("Hp: %d \n", Player->maxHp);
    printf("You have %d skill points remaining. What would you like to increase?\n", skillPoints);
    fgets(statUp,sizeof(statUp), stdin);
    system("clear");
    skillPoints = skillPoints - 1;
    if(strcasecmp("Attack\n",statUp) == 0)
      Player->atk = Player->atk + 1;
    else if(strcasecmp("Defense\n",statUp) == 0)
      Player->def = Player->def + 1;
    else if(strcasecmp("Speed\n",statUp) == 0)
      Player->spd = Player->spd + 1;
    else if(strcasecmp("Hp\n",statUp) == 0)
      Player->maxHp = Player->maxHp + 1;
    else{
      printf("Sorry, that is not a valid stat.\n");
      skillPoints = skillPoints + 1;
    }
  }
  Player->hp = Player->maxHp;
}

void pickUpLoot(){
  system("clear");
  printf("You found the %s %s %s %s! (%d)\n", Part1[DRoom->loot.part1], Part2[DRoom->loot.part2], wType[DRoom-> loot.type], Part3[DRoom ->loot.part3], DRoom->loot.lvl);
  sleep(1);
  if (Player->weaponNum < 2){
    printf("You picked up the %s %s %s %s! (%d)\n", Part1[DRoom->loot.part1], Part2[DRoom->loot.part2], wType[DRoom-> loot.type],Part3[DRoom->loot.part3], DRoom->loot.lvl);
    if(Player->weaponNum == 0){
      //free(&Player->slot1);
      Player->slot1 = DRoom->loot;
      Player->weaponNum = 1;
      Player->atk = Player->atk + Player->slot1.attk;
    }
    else if(Player->weaponNum == 1){
      //free(&Player->slot2);
      Player->slot2 = DRoom->loot;
      Player->atk = Player->atk + Player->slot2.attk;
      Player->weaponNum = 2;
    }
  }
  else{
    while(1){
      printf("Your inventory is full. Would you like to discard a Weapon?\n");
      printf("Your slot 1 weapon gives you +%d to attack\n",Player->slot1.attk);
      printf("Your slot 2 weapon gives you +%d to attack\n",Player->slot2.attk);
      printf("The loot weapon would give  you +%d to attack\n",DRoom->loot.attk);
      fgets(input,sizeof(input),stdin);
      if(strcasecmp(input,"Yes\n") == 0){
	printf("Which slot would you like to discard?\n");
	fgets(input,sizeof(input),stdin);
	if(strcasecmp("1\n",input) == 0 || strcasecmp("Slot 1\n",input) == 0){
	  Player->atk = Player->atk - Player->slot1.attk;
	  //free(&Player->slot1);
	  Player->slot1 = DRoom->loot;
	  Player->atk = Player->atk + Player->slot1.attk;
	  break;
	}
	else if(strcasecmp("2\n",input) == 0 || strcasecmp("Slot 2\n",input) == 0){
	  Player->atk = Player->atk - Player->slot2.attk;
	  //free(&Player->slot2);
	  Player->slot2 = DRoom->loot;
	  Player->atk = Player->atk + Player->slot2.attk;
	  break;
	}
	else{
	  printf("Sorry, I did not understand what you said\n");
	}
      }
      else{
	//free(&DRoom -> loot);
	break;
      }
    }
  }
}


void dump(){
  int fd = open("savefile.file", O_RDWR | O_CREAT, 0666);
  write(fd, Player, sizeof(Character));
  //DRoom -> Enemy = *Enemy
  write(fd, DRoom, sizeof(Room));
  close(fd);
}

int LoadChooser(){
  char onlineconfirm[256];
  printf("Would you like to load an online save? You must have already saved to the server from which you are loading!\n");
  fgets(onlineconfirm, sizeof(onlineconfirm), stdin);
  if (strcasecmp("Yes\n", onlineconfirm) == 0){
    OLoad();
    return 0;
  }
  else{
    return load();
  }
  // return 0;
}


int load(){
  int fd = open("savefile.file", O_RDWR);
  if(fd == -1)
    return -1;
  Player = (Character *) calloc(1, sizeof(Character));
  read(fd, Player, sizeof(Character));
  DRoom = (Room *) calloc(1, sizeof(Room));
  read(fd, DRoom, sizeof(Room));
  Enemy = &(DRoom -> Enemy);
  close(fd);
  return 0;
}

int OSave(int argz, char *ipadress) {

  int socket_id;
  char buffer[256];
  int i, b;
  strcpy( buffer, "save\n");
  struct sockaddr_in sock;

  //make the server socket for reliable IPv4 traffic 
  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  //set up the server socket struct
  //Use IPv4 
  sock.sin_family = AF_INET;
  
  //Client will connect to address in ipadress, need to translate that IP address to binary
  inet_aton( &ipadress, &(sock.sin_addr) );
  //set the port to listen on, htons converts the port number to network format
  sock.sin_port = htons(24607);
  
  //connect to the server
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("Connect returned: %d\n", c);
  write( socket_id, buffer, strlen(buffer));
  b = read( socket_id, buffer, sizeof(buffer) );
  printf("%s", buffer);
  if (strcmp(buffer, "send") == 0){
    int fd = open("savefile.file", O_RDWR | O_CREAT, 0666);
    read(fd, buffer, sizeof(buffer));
    write( socket_id, buffer, strlen(buffer));   
    close(fd);	
    }

    close(socket_id);

    return 0;
}

int OLoad(int argz, char *ipadress) {

  int socket_id;
  char buffer[256];
  int i, b;
  strcpy( buffer, "load\n");
  struct sockaddr_in sock;

  //make the server socket for reliable IPv4 traffic 
  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  printf("Soket file descriptor: %d\n", socket_id);

  //set up the server socket struct
  //Use IPv4 
  sock.sin_family = AF_INET;
  
  //Client will connect to address in ipadress, need to translate that IP address to binary
  inet_aton( &ipadress, &(sock.sin_addr) );
  //set the port to listen on, htons converts the port number to network format
  sock.sin_port = htons(24607);
  
  //connect to the server
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("Connect returned: %d\n", c);
  write( socket_id, buffer, strlen(buffer));
  b = read( socket_id, buffer, strlen(buffer) );
  int fd = open("savefile.file", O_RDWR | O_CREAT, 0666);
  write(fd, buffer, sizeof(buffer));   
  close(fd);	

  close(socket_id);
  
  load();
  return 0;
}
