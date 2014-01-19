#include "database.h"

static int callback(void * comm, int argc, char **argv, char **azColName) {
    //Callback function...
    int type = *(int *) comm;
    if( type == NO_CALLBACK ) {
        return 0;
    }
    else if( type == U_EXISTS_CALLBACK ) {
        //Only calls back on successful SELECT
        //The user exists so we change comm
        *(int *)comm = USER_EXISTS;
    } else if( type == GAME_EXISTS_CALLBACK ) {
        *(int *)comm = GAME_EXISTS;  
    } else if( type == GAME_INFO_CALLBACK ) {
        free(comm);
        /*
           comm = (game_info *) calloc( sizeof(game_info), 1);
           ((game_info *) comm)->turn = atoi(argv[2]);
           ((game_info *) comm)->u1wins = atoi(argv[3]);
           ((game_info *) comm)->u2wins = atoi(argv[4]);
           ((game_info *) comm)->dist = atoi(argv[6]);
           printf("%d %d comms\n",  
           ((game_info *) comm)->u1wins, ((game_info *) comm)->u2wins );
           */
        gi.turn = atoi(argv[2]);
        gi.u1wins = atoi(argv[3]);
        gi.u2wins = atoi(argv[4]);
        gi.dist = atoi(argv[6]);
    } else if( type == GET_TURN_CALLBACK ) {
        *(int *)comm = atoi(argv[2]);
    } else if( type == GET_KEY_CALLBACK ) {
        *(int *)comm = atoi(argv[5]);
    } else if( type == VALIDATE_USER_CALLBACK ) {
        *(int *)comm = VALID;
    } else if( type == GET_GAMES_IN_PROGRESS_CALLBACK ) {

        int i = gip_hold->number_of_games;
        db_game_data ** temp_games = calloc(i+1, sizeof(db_game_data *));
        int j = 0;
        while(j < i) {
            temp_games[j] = gip_hold->games[j];
            j++;
        }
        temp_games[i] = malloc(sizeof(db_game_data));
        (temp_games[i])->type = GAMES_IN_PROG;
        strcpy((temp_games[i])->u1, argv[0]);
        strcpy((temp_games[i])->u2, argv[1]);
        (temp_games[i])->turn = atoi(argv[2]);
        free(gip_hold->games);
        gip_hold->games = temp_games;
        gip_hold->number_of_games++; 
    }
    return 0;
}
void db_init() {
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
void db_close() {
    close_sems();
}
void close_sems() {
    semctl(writesem, 0, IPC_RMID);
    semctl(createsem, 0, IPC_RMID);
    semctl(readsem, 0, IPC_RMID);
}
int db_create_user(char * name, char * password) {
    //Aquire the create lock
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;
    semop(createsem, &sb, 1);

    //printf("Have create lock\n");
    if( db_user_exists(name) ) {
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
    char * execStr = composeAddUser(name, password);
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
int db_validate_user(char * name, char * password) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    //Type is sent with information about whose callback routine to call
    void * type = malloc(sizeof(int));
    *(int *)type = VALIDATE_USER_CALLBACK;
    char * execStr = compose_validate_user(name, password);
    rc = sqlite3_exec(db, execStr, callback, type, &zErrMsg);
    free(execStr);
    //Type returns containing whether the user exists or not
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);

    return ( *(int *)type == VALID );
}

/* Assume that the data has been locked before entering this function
 *
 */
int db_user_exists(char * name) {
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
db_game_data_wr * db_games_in_progress(char * name) {
    struct sembuf sbT;
    sbT.sem_num = 0;
    sbT.sem_flg = SEM_UNDO;
    sbT.sem_op = -1;

    //Wait for no writer to have writing lock
    semop(writesem, &sbT, 1);
    //printf("Temp got write\n");

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    //Type is sent with information about whose callback routine to call
    void * type = malloc(sizeof(int));
    *(int *)type = GET_GAMES_IN_PROGRESS_CALLBACK;
    char * execStr = composeGetGamesOf(name);
    printf("%s\n", execStr);
    rc = sqlite3_exec(db, execStr, callback, type, &zErrMsg);
    free(execStr);
    //Type returns containing whether the user exists or not
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    //Games in progress are stored in gip_hold

    db_game_data_wr * ret = gip_hold;
    gip_hold = NULL;
    sbT.sem_op = 1;
    semop(writesem, &sbT, 1);

    return ret;

}
int db_get_key( char * name, char * opponent ) {
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


    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    //Type is sent with information about whose callback routine to call
    void * type = malloc(sizeof(int));
    *(int *)type = GET_KEY_CALLBACK;
    char * execStr = composeGetGameInfo(name, opponent);
    rc = sqlite3_exec(db, execStr, callback, type, &zErrMsg);
    free(execStr);
    //Type returns containing whether the user exists or not
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    sbR.sem_op = -1;
    semop(readsem, &sbR, 1);

    return *(int *)type;
}
int db_my_turn( char * name, char * opponent ) {
    int my_turn;
    if( strcmp(name, opponent) > 0 ) {
        my_turn = U1_TURN;
    } else {
        my_turn = U2_TURN;
    }   
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


    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    //Type is sent with information about whose callback routine to call
    void * type = malloc(sizeof(int));
    *(int *)type = GET_TURN_CALLBACK;
    char * execStr = composeGetGameInfo(name, opponent);
    rc = sqlite3_exec(db, execStr, callback, type, &zErrMsg);
    free(execStr);
    //Type returns containing whether the user exists or not
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    sbR.sem_op = -1;
    semop(readsem, &sbR, 1);

    return ( *(int *)type == my_turn);

}
void db_update_game( struct game_data * gd ) {
    char u1[50];
    char u2[50];

    if( strcmp(gd->from, gd->to) > 0 ) {
        strcpy(u1, gd->to);
        strcpy(u2, gd->from);
    } else {
        strcpy(u2, gd->to);
        strcpy(u1, gd->from);
    }   
    if( db_game_exists(u1, u2) ) {
        struct sembuf sbT;
        sbT.sem_num = 0;
        sbT.sem_flg = SEM_UNDO;
        sbT.sem_op = -1;

        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;

        semop(writesem, &sbT, 1);
        //Aquired writing lock
        rc = sqlite3_open("data.db",&db);
        assert( !rc );
        //Type is sent with information about whose callback routine to call
        void * type = malloc(sizeof(int));
        *(int *)type = GAME_INFO_CALLBACK; 
        char * execStr = composeGetGameInfo(u1, u2); 
        rc = sqlite3_exec(db, execStr, callback, type, &zErrMsg);
        free(execStr);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        //turn = 0, u1 submitted
        //
        //printf("%lu\n", type);
        //game_info * gi = (game_info *) type;
        if( gi.turn ){
            //User two is gi
            if( gi.dist > gd->dist ){
                gi.u2wins++;
            } else {
                gi.u1wins++;
            }
        } else {
            if( gi.dist > gd->dist ){
                gi.u1wins++;
            } else {
                gi.u2wins++;
            }
        }
        gi.turn = !gi.turn;
        execStr = composeUpdateWins(u1, u2, gi.u1wins, gi.u2wins);
        void * t = malloc(sizeof(int));
        *(int *) t = NO_CALLBACK;
        rc = sqlite3_exec(db, execStr, callback, t, &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        free(execStr);
        *(int *) t = NO_CALLBACK;
        execStr = composeAddChallenge(u1, u2, gd->nextdist, gi.turn, gd->genkey);
        rc = sqlite3_exec(db, execStr, callback, t, &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        free(execStr);
        sqlite3_close(db);
        //Release lock
        //
        sbT.sem_op = 1;
        semop(writesem, &sbT, 1);

    } else {
        //Create game
        db_create_game(gd);
    }
}

int db_game_exists(char * a, char * b) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    //Type is sent with information about whose callback routine to call
    void * type = malloc(sizeof(int));
    *(int *)type = GAME_EXISTS_CALLBACK; 
    char * execStr = composeGameExists(a, b); 
    rc = sqlite3_exec(db, execStr, callback, type, &zErrMsg);
    free(execStr);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);

    return ( *(int *)type == GAME_EXISTS);
}

void db_create_game( struct game_data * gd ) {
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
/*
int main(int argc, char ** argv) {
    struct game_data * gd = malloc(sizeof(struct game_data));
    db_init();
    strcpy(gd->to, "adaron");
    strcpy(gd->from, "john");
    gd->genkey = 1231;
    gd->dist = 123;
    db_update_game(gd);
    strcpy(gd->to, "john");
    strcpy(gd->from, "phillip");
    gd->dist = 12763;
    db_update_game(gd);

    printf("%d\n", db_game_exists("adaron", "john"));
    db_games_in_progress("john");

    exit(0);
    //printf("Initing\n");
    //createUser("aaron", 2013);
    int i = 0;
    for(int i = 0; i < 10; i++) {
        int p = fork();
        if(p == 0) {
            db_create_user("aaron", "yolo");
            db_user_exists("aaron");
            exit(0);
        }
    }
    printf("Exists %d\n", db_user_exists("aaron"));
    close_sems();
    return 0;
}*/
