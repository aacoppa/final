#include "database.h"
#include <unistd.h>

static int callback(void * comm, int argc, char **argv, char **azColName) {
    //Callback function...
    int type = *(int *) comm;
    if( type == NO_CALLBACK ) {
        return 0;
    }
    else if( type == U_EXISTS_CALLBACK ) {
        //The user exists so we change comm
        *(int *)comm = USER_EXISTS;
    }
    return 0;
}
void init() {
    char * execStr = "DROP TABLE IF EXISTS users; CREATE TABLE users ( username TEXT, passhash INTEGER );";
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    rc = sqlite3_exec(db, execStr, callback, NO_CALLBACK, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    
    //Set up lock, allows one thread in critical section at a time
    int semkey = 714;
    semd = semget(semkey, 1, IPC_CREAT | IPC_CREAT | 0666);
    union semun su;
    su.val = 1;
    semctl(semd, 0, SETVAL, su);
}
int createUser(char * name, int passhash) {
    //Aquire the lock
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;
    semop(semd, &sb, 1);
    
    printf("Have lock\n");
    if( userExists(name) ) {
        //Release lock
        printf("Released lock, exists\n");
        sb.sem_op = 1;
        semop(semd, &sb, 1);
        return CREAT_FAILED;
    }
    char * execStr = malloc(400 * sizeof(char *));
    strcat(execStr, "INSERT INTO users VALUES ( \"");
    strcat(execStr, name);
    strcat(execStr, "\", ");
    char c[20];
    sprintf(c, "%d", passhash);
    strcat(execStr, c);
    strcat(execStr, ");");

    printf("%s\n", execStr);
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    rc = sqlite3_exec(db, execStr, callback, NO_CALLBACK, &zErrMsg);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    free(execStr);
    //Release the lock
    printf("Released lock\n");
    sb.sem_op = 1;
    semop(semd, &sb, 1);

    return CREAT_SUCC;
}

/* Assume that the data has been locked before entering this function
 *
 */
int userExists(char * name) {
    char * execStr = malloc(400 * sizeof(char *));
    strcat(execStr, "SELECT * FROM USERS WHERE username=\"");
    strcat(execStr, name);
    strcat(execStr, "\";");

    printf("%s\n", execStr);
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    //Type is sent with information about whose callback routine to call
    void * type = malloc(sizeof(int));
    *(int *)type = U_EXISTS_CALLBACK;
    rc = sqlite3_exec(db, execStr, callback, type, &zErrMsg);
    //Type returns containing whether the user exists or not
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    return (*(int *)type == USER_EXISTS);
}
int main(int argc, char ** argv) {
    init();
    int i = 0;
    for(int i = 0; i < 10; i++) {
        int p = fork();
        if(p == 0) {
            createUser("aaron", 2013);
            exit(0);
        }
    }
    printf("Exists %d\n", userExists("aaron"));
    return 0;
}
