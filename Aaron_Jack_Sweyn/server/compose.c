/* Compose SQL statements
 * to be executed from database.c
 *
 */
#include "compose.h"

char * composeInitTables() {
    char * execStr = malloc(400);
    strcpy(execStr, "DROP TABLE IF EXISTS users; CREATE TABLE users ( username TEXT, password TEXT); DROP TABLE IF EXISTS game_data; CREATE TABLE game_data ( u1 TEXT, u2 TEXT, turn INTEGER, u1wins INTEGER, u2wins INTEGER, key BLOB, dist INTEGER)");
    return execStr;
}

char * composeUserExists(char * name) {
    char * execStr = malloc(400 * sizeof(char *));
    strcat(execStr, "SELECT * FROM USERS WHERE username=\"");
    strcat(execStr, name);
    strcat(execStr, "\";");
    return execStr;
}
char * composeAddUser(char * name, char * password) {
    char * execStr = malloc(400);
    strcat(execStr, "INSERT INTO users VALUES ( \"");
    strcat(execStr, name);
    strcat(execStr, "\", ");
    //char c[20];
    //sprintf(c, "%d", passhash);
    strcat(execStr, password);
    strcat(execStr, ");");
    return execStr;
}
char * composeGameExists(char * u1, char * u2) {
    char * execStr = malloc(200);
    strcpy(execStr, "SELECT * FROM game_data WHERE u1=\"");
    strcat(execStr, u1);
    strcat(execStr, "\" and u2=\"");
    strcat(execStr, u2);
    strcat(execStr, "\";");
    return execStr;
}

char * compose_validate_user(char * user, char * passwd) {
    char * execStr = malloc(200);
    strcpy(execStr, "SELECT * FROM users WHERE username=\"");
    strcat(execStr, user);
    strcat(execStr, "\" and password=\"");
    strcat(execStr, passwd);
    strcat(execStr, "\";");
    return execStr;

}
char * composeGetGameInfo(char * u1, char * u2) {
    char * execStr = malloc(400 * sizeof(char *));
    strcat(execStr, "SELECT * FROM game_data WHERE u1=\"");
    strcat(execStr, u1);
    strcat(execStr, "\" and u2=\"");
    strcat(execStr, u2);
    strcat(execStr, "\";");
    return execStr;

}
char * composeUpdateWins(char * u1, char * u2, int u1wins, int u2wins) {
    char * execStr = malloc(200);
    strcpy(execStr, "UPDATE game_data SET u1wins=\"");
    char c[20];
    sprintf(c, "%d", u1wins);
    strcat(execStr, c);
    strcat(execStr, "\", u2wins=\"");
    sprintf(c, "%d", u2wins);
    strcat(execStr, c);
    strcat(execStr, "\" WHERE u1=\"");
    strcat(execStr, u1);
    strcat(execStr, "\" and u2=\"");
    strcat(execStr, u2);
    strcat(execStr, "\";");
    return execStr;

}
char * composeAddChallenge(char * u1, char * u2, int dist, int newTurn, int key) {
    char * execStr = malloc(200);
    strcpy(execStr, "UPDATE game_data SET key=\"");
    char c[20];
    sprintf(c, "%d", key);
    strcat(execStr, c);
    strcat(execStr, "\", dist=\"");
    sprintf(c, "%d", dist);
    strcat(execStr, c);
    strcat(execStr, "\", turn=\"");
    sprintf(c, "%d", newTurn);
    strcat(execStr, c);
    strcat(execStr, "\" WHERE u1=\"");
    strcat(execStr, u1);
    strcat(execStr, "\" and u2=\"");
    strcat(execStr, u2);
    strcat(execStr, "\";");
    return execStr;
}
char * composeNewGameEntry(struct game_data * gd) {
    char * execStr = malloc(200);
    char u1[50];
    char u2[50];
    
    if( strcmp(gd->from, gd->to) > 0 ) {
        strcpy(u1, gd->to);
        strcpy(u2, gd->from);
    } else {
        strcpy(u2, gd->to);
        strcpy(u1, gd->from);

    }
    strcpy(execStr, "INSERT INTO game_data VALUES ( \"");
    strcat(execStr, u1);
    strcat(execStr, "\" , \"");
    strcat(execStr, u2);
    strcat(execStr, "\" , "); 
    if( strcmp(gd->from, u1) == 0 ) {
        strcat(execStr, "2");
    } else {
        strcat(execStr, "1");
    }
    strcat(execStr, " , 0 , 0 , ");
    
    char c[20];
    sprintf(c, "%d", gd->dist);
    strcat(execStr, c);
    strcat(execStr, " , ");
    sprintf(c, "%d", gd->genkey);
    strcat(execStr, c);
    strcat(execStr, " );");
    return execStr;
}
/*
int main() {
    printf("%s\n", composeAddChallenge("aaron", "john", 1, 2, 3));
}*/
