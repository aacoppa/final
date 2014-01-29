#include "server.h"

int rooms[NUM_ROOMS][NUM_PLAYERS_PER_ROOM];

int main() {
  int socket_id, socket_client;
  char buffer[256];
  int i, b;

  int roomNum;
  
  struct sockaddr_in server;
  socklen_t socket_length;

  //Set up and bind socket
  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  printf("Socket file descriptor: %d\n", socket_id);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;  
  server.sin_port = htons(24601);
  i= bind( socket_id, (struct sockaddr *)&server, sizeof(server) );
  i =  listen( socket_id, 1 );

  //Initiate Rooms
  int x,y;
  for(x = 0;x < NUM_ROOMS;x++){
    for(y = 0;y < NUM_PLAYERS_PER_ROOM;y++){
      rooms[x][y] = 0;
    }
  }

  //acept connections continuously
  while(1) {
    printf("Accepting a connection\n");
    socket_length = sizeof(server); 
    socket_client = accept(socket_id, (struct sockaddr *)&server, &socket_length);
    printf("accepted connection %d\n",socket_client);
    i = fork();
    if ( i == 0 )
      subserver(socket_client);
    else 
      close(socket_client);
      
    printf("Waiting for new connection\n");
  }
}

void subserver( int socket_client ) {
    char buffer[256];
    int b, i;
    b = 0;

    //do client stuff continuously
    while (1) {
      //read from the client
      b = read( socket_client, buffer, sizeof(buffer) );
      printf("Received: %s\n", buffer);

      if ( strncmp(buffer, JOIN_ROOM, sizeof(JOIN_ROOM)) == 0 ){
	//Add client to room requested and send
	//client a confirmation or failure
	int room = (int)(buffer[sizeof(JOIN_ROOM)]);
	int join = joinRoom(socket_client,room);
	printf("%d joined room %d", socket_client,room);
	//Send client confirmation or rejection
      }
      if ( strncmp(buffer, CREATE_ROOM, sizeof(CREATE_ROOM)) == 0 ){
	//Create Room and send client his/her room number
	//If room not created, send failure message
	int room = createRoom(socket_client);
	printf("%d created room %d", socket_client,room);
	//Send client confirmation or rejection
      }
      if ( strncmp(buffer, LEAVE_ROOM, sizeof(buffer)) == 0 ){
	//Remove client from room
	int room = (int)(buffer[sizeof(JOIN_ROOM)]);
	int left = leaveRoom(socket_client,room);
	printf("%d left room %d", socket_client,room);
	//Send client confirmation
      }
      if ( strncmp(buffer, GET_MAZE, sizeof(buffer)) == 0 )
	//get maze info from generator and send to clients
        return;
      if ( strncmp(buffer, UPDATE_SCORE, sizeof(buffer)) == 0 )
	//update score in room
	return;
      if ( strncmp(buffer, EXIT_CLIENT, sizeof(buffer)) == 0 ){
	//Client exited. Exit subserver
        close(socket_client);
	exit(EXIT_SUCCESS);
      }
      
      write( socket_client, buffer, strlen(buffer));
    }
}


int joinRoom(int socket_client,int roomNum){
  return -1;
}
/* Makes client the "leader" of first
   empty room.
   Returns: room number if successful,
            -1 if not successful. */
int createRoom( int socket_client ) {
  int roomNum = findEmptyRoom();
  if(roomNum == -1)
    return -1;
  else{
    rooms[roomNum][0] = socket_client;
    return roomNum;
  }
}
/* Finds first empty room.
   Returns: room number if successful,
            -1 if not sucessful.*/
int findEmptyRoom(){
  int i;
  for(i = 0;i < NUM_ROOMS;i++){
    if(rooms[i][0] == 0)
      return i;
  }
  return -1;
}
int leaveRoom(int socket_client,int roomNum){
  int room[NUM_PLAYERS_PER_ROOM];
  int i;
  for(i = 0;i < NUM_PLAYERS_PER_ROOM;i++)
    room[i] = rooms[roomNum][i];
  for(i = 0;i < NUM_PLAYERS_PER_ROOM;i++){
    if(room[i] == socket_client){
      //Shift other client_ids over
      int j;
      for(j = i + 1; j < NUM_PLAYERS_PER_ROOM;j++)
	room[j-1] = room[j];
      room[NUM_PLAYERS_PER_ROOM - 1] = 0;
    }
  }
  return 1;
}
int getMaze(int socket_client){
  return -1;
}
int updateScore(int socket_client,int roomNum,int score){
  return -1;
}
