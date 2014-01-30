/* Compose SQL statements
 * to be executed from database.c
 *
 * HANDLING is done is database:
 * static int callback();
 */
#include "compose.h"

/* All simple string concatanation
 * nothing interest except sql code
 *
 */
char * compose_init_tables() {
    char * execStr = malloc(400);
    //strcpy(execStr, "DROP TABLE IF EXISTS users; CREATE TABLE users ( username TEXT, password TEXT); DROP TABLE IF EXISTS game_data; CREATE TABLE game_data ( u1 TEXT, u2 TEXT, turn INTEGER, u1wins INTEGER, u2wins INTEGER, key BLOB, dist INTEGER);");

    strcpy(execStr, "CREATE TABLE users ( username TEXT, password TEXT); CREATE TABLE game_data ( u1 TEXT, u2 TEXT, turn INTEGER, u1wins INTEGER, u2wins INTEGER, key BLOB, dist INTEGER, last INTEGER);");
    return execStr;
}

char * compose_user_exists(char * name) {
    char * execStr = calloc(1, 400);
    strcat(execStr, "SELECT * FROM USERS WHERE username=\"");
    strcat(execStr, name);
    strcat(execStr, "\";");
    return execStr;
}
char * compose_add_user(char * name, char * password) {
    char * execStr = calloc(400, 1);
    strcat(execStr, "INSERT INTO users VALUES ( \"");
    strcat(execStr, name);
    strcat(execStr, "\", \"");
    //char c[20];
    //sprintf(c, "%d", passhash);
    strcat(execStr, password);
    strcat(execStr, "\");");
    return execStr;
}
char * compose_get_games_of(char * name) {
    char * execStr = malloc(200);
    strcpy(execStr, "SELECT * FROM game_data WHERE u1=\"");
    strcat(execStr, name);
    strcat(execStr, "\" or u2=\"");
    strcat(execStr, name);
    strcat(execStr, "\";");
    return execStr;
}
char * compose_game_exists(char * a, char * b) {
    char u1[50];
    char u2[50];
    if( strcmp(a, b) < 0) {
        strcpy(u1, a);
        strcpy(u2, b);
    } else {
        strcpy(u1, b);
        strcpy(u2, a);
    }

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
char * compose_get_game_info(char * a, char * b) {
    char u1[50];
    char u2[50];
    if( strcmp(a, b) < 0) {
        strcpy(u1, a);
        strcpy(u2, b);
    } else {
        strcpy(u1, b);
        strcpy(u2, a);
    }


    char * execStr = malloc(400 * sizeof(char *));
    strcat(execStr, "SELECT * FROM game_data WHERE u1=\"");
    strcat(execStr, u1);
    strcat(execStr, "\" and u2=\"");
    strcat(execStr, u2);
    strcat(execStr, "\";");
    return execStr;

}
char * compose_update_wins(char * u1, char * u2, int u1wins, int u2wins, int last) {
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
char * compose_add_challenge(char * u1, char * u2, int dist, int newTurn, int key) {
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
    strcat(execStr, "\", last=\"");
    strcat(execStr, c);
    strcat(execStr, "\" WHERE u1=\"");
    strcat(execStr, u1);
    strcat(execStr, "\" and u2=\"");
    strcat(execStr, u2);
    strcat(execStr, "\";");
    return execStr;
}
char * compose_new_game_entry(cli_upload_game * gd) {
    char * execStr = malloc(200);
    char u1[50];
    char u2[50];
    int turn = 0;
    if( strcmp(gd->name, gd->opponent) < 0 ) {
        strcpy(u1, gd->name);
        strcpy(u2, gd->opponent);
    } else {
        strcpy(u2, gd->name);
        strcpy(u1, gd->opponent);

    }
    strcpy(execStr, "INSERT INTO game_data VALUES ( \"");
    strcat(execStr, u1);
    strcat(execStr, "\" , \"");
    strcat(execStr, u2);
    strcat(execStr, "\" , "); 
    if( strcmp(gd->name, u1) == 0 ) {
        strcat(execStr, "1");
    } else {
        strcat(execStr, "0");
    }
    strcat(execStr, " , 0 , 0 , ");
    
    char c[20];
    sprintf(c, "%d", gd->dist);
    strcat(execStr, c);
    strcat(execStr, " , ");
    char d[20];
    sprintf(d, "%d", gd->key);
    strcat(execStr, d);
    strcat(execStr, ", 2 );");
    return execStr;
}
