#include "database.h"

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
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    char * execStr = composeInitTables();
    rc = sqlite3_exec(db, execStr, callback, NO_CALLBACK, &zErrMsg);
    free(execStr);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    //Set up lock, allows one thread in critical section at a time
    int semkey = 7141;
    createsem = semget(semkey, 1, IPC_CREAT | IPC_CREAT | 0666);
    union semun su;
    su.val = 1;
    semctl(createsem, 0, SETVAL, su);

    semkey = 7151;
    writesem = semget(semkey, 1, IPC_CREAT | IPC_CREAT | 0666);
    union semun suT;
    suT.val = 1;
    semctl(writesem, 0, SETVAL, suT);

    semkey = 7161;
    readsem = semget(semkey, 1, IPC_CREAT | IPC_CREAT | 0666);
    union semun suTT;
    suTT.val = 0;
    semctl(readsem, 0, SETVAL, suTT);
}
void closeSems() {
    semctl(writesem, 0, IPC_RMID);
    semctl(createsem, 0, IPC_RMID);
    semctl(readsem, 0, IPC_RMID);
}
int createUser(char * name, int passhash) {
    //Aquire the create lock
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;
    semop(createsem, &sb, 1);
    
    //printf("Have create lock\n");
    if( userExists(name) ) {
        //Release create lock
        printf("Released create lock, exists\n");
        sb.sem_op = 1;
        semop(createsem, &sb, 1);
        return CREAT_FAILED;
    }

    //Now we aquire writing lock since we are writing
    //No more readers may enter critical section

    //printf("Waiting for write lock\n");
    struct sembuf sbT;
    sbT.sem_num = 0;
    sbT.sem_flg = SEM_UNDO;
    sbT.sem_op = -1;
    semop(writesem, &sbT, 1);
    
    //printf("Have write lock\n");
    //Wait for all readers to be clear
    struct sembuf sbR;
    sbR.sem_num = 0;
    sbR.sem_flg = SEM_UNDO;
    sbR.sem_op = 0;
    semop(readsem, &sbR, 1);

    //printf("All readers clear\n");
    //Wait for all readers to be clear
    //Now we can write, we are the only one here
    
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    char * execStr = composeAddUser(name, passhash);
    rc = sqlite3_exec(db, execStr, callback, NO_CALLBACK, &zErrMsg);
    free(execStr);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    //Release the lock
    printf("Releasing create and write lock\n");
    sb.sem_op = 1;
    semop(createsem, &sb, 1);
    
    sbT.sem_op = 1;
    semop(writesem, &sbT, 1);

    return CREAT_SUCC;
}

/* Assume that the data has been locked before entering this function
 *
 */
int userExists(char * name) {
    //wait for write lock to be free
    struct sembuf sbT;
    sbT.sem_num = 0;
    sbT.sem_flg = SEM_UNDO;
    sbT.sem_op = -1;
    
    struct sembuf sbR;
    sbR.sem_num = 0;
    sbR.sem_flg = SEM_UNDO;
    sbR.sem_op = 1;
    
    //Wait for no writer to have writing lock
    semop(writesem, &sbT, 1);
    //printf("Temp got write\n");
    //Add to reading semaphore
    semop(readsem, &sbR, 1);
    
    //printf("Temp got write\n");
    sbT.sem_op = 1;
    semop(writesem, &sbT, 1);
    //printf("Aquired reader lock and no writer\n");
    //printf("%s\n", execStr);
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    //Type is sent with information about whose callback routine to call
    void * type = malloc(sizeof(int));
    *(int *)type = U_EXISTS_CALLBACK;
    char * execStr = composeUserExists(name);
    rc = sqlite3_exec(db, execStr, callback, type, &zErrMsg);
    free(execStr);
    //Type returns containing whether the user exists or not
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    //One less writer
    sbR.sem_op = -1;
    semop(readsem, &sbR, 1);
    //printf("Decrementing read lock\n");

    return (*(int *)type == USER_EXISTS);
}
/*
void updateGame( struct game_data * gd ) {
        
    if( gameExists(gd->from, gd->to) ) {

    } else {
        //Create game
        createGame(gd);
    }
}
*/
void createGame( struct game_data * gd ) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    //Type is sent with information about whose callback routine to call
    void * type = malloc(sizeof(int));
    *(int *)type = NO_CALLBACK; //Don't need a callback
    char * execStr = composeNewGameEntry(gd);
    rc = sqlite3_exec(db, execStr, callback, type, &zErrMsg);
    free(execStr);
    if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}   
int main(int argc, char ** argv) {
    struct game_data * gd = malloc(sizeof(struct game_data));
    init();
    strcpy(gd->to, "Aaron");
    strcpy(gd->from, "John");
    gd->genkey = 1231;
    gd->time = 123;
    createGame(gd);
    exit(0);
    //printf("Initing\n");
    //createUser("aaron", 2013);
    int i = 0;
    for(int i = 0; i < 10; i++) {
        int p = fork();
        if(p == 0) {
            createUser("aaron", 2013);
            userExists("aaron");
            exit(0);
        }
    }
    printf("Exists %d\n", userExists("aaron"));
    closeSems();
    return 0;
}
