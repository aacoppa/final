/* Compose SQL statements
 * to be executed from database.c
 *
 */
#include "compose.h"

char * composeInitTables() {
    char * execStr = "DROP TABLE IF EXISTS users; CREATE TABLE users ( username TEXT, passhash INTEGER ); DROP TABLE IF EXISTS game_data; CREATE TABLE game_data ( u1 TEXT, u2 TEXT, turn INTEGER, u1wins INTEGER, u2wins INTEGER, lastkey BLOB, time INTEGER)";
    return execStr;
}

char * composeUserExists(char * name) {
    char * execStr = malloc(400 * sizeof(char *));
    strcat(execStr, "SELECT * FROM USERS WHERE username=\"");
    strcat(execStr, name);
    strcat(execStr, "\";");
    return execStr;
}
char * composeAddUser(char * name, int passhash) {
    char * execStr = malloc(400 * sizeof(char *));
    strcat(execStr, "INSERT INTO users VALUES ( \"");
    strcat(execStr, name);
    strcat(execStr, "\", ");
    char c[20];
    sprintf(c, "%d", passhash);
    strcat(execStr, c);
    strcat(execStr, ");");
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
    sprintf(c, "%d", gd->time);
    strcat(execStr, c);
    strcat(execStr, " , ");
    sprintf(c, "%d", gd->genkey);
    strcat(execStr, c);
    strcat(execStr, " );");
    return execStr;
}
