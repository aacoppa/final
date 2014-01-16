 /*
  * Forking server
  * Read requests and respond
  * No real constant connection, except for handshakes
  * Instead we handle reconnections
  *
  *
  */
#include <server_side.h>
#include "database.h"
int main(int argc, char ** argv) {
    start();
}
void start() {
    //Begins the back end server
    init(); //Init database
}
void listenForNewConnections() {
    //Listen for a request

}
void handleRequestType() {

}

void acceptNewConnection() {
    //Handle a connection, maybe a handshake, then end
}
void handleConnectionFault() {

}

