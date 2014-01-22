#include "database.h"

static int callback(void * in, int argc, char **argv, char **azColName) {
    // This function is only called when there is something in the database
    // that matches the query
    
    //Called multiple times if there are multiple matches 
    int type = *(int *)in; //Instead of user comm as a void * we use it
    //As a placeholder to say what kind of action to take
    //
    int * comm = in;
    if( type == NO_CALLBACK ) {
        return 0;
    }
    else if( type == U_EXISTS_CALLBACK ) {
        *comm = USER_EXISTS; 
    } else if( type == GAME_EXISTS_CALLBACK ) {
        printf("Game does exists\n");
        *comm = GAME_EXISTS;  
    } else if( type == GAME_INFO_CALLBACK ) {
        //Fill out the global LOCKED game_info * gi struct
        free(comm);
        gi.turn = atoi(argv[2]); 
        gi.u1wins = atoi(argv[3]);
        gi.u2wins = atoi(argv[4]);
        gi.dist = atoi(argv[6]);
    } else if( type == GET_TURN_CALLBACK ) {
        *comm = atoi(argv[2]);
    } else if( type == GET_KEY_CALLBACK ) {
        *comm = atoi(argv[5]);
    } else if( type == VALIDATE_USER_CALLBACK ) {
        *comm = VALID;
    } else if( type == GET_GAMES_IN_PROGRESS_CALLBACK ) {
        //gip_hold is also global LOCKED
        //update it everytime with another game 
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

void db_execute(char * exec_str, void ** t) {
    //Execute sql statements
    void * type = *t;
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open("data.db",&db);
    assert( !rc );
    printf("%s\n", exec_str);
    rc = sqlite3_exec(db, exec_str, callback, type, &zErrMsg);
    free(exec_str);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

void db_init() {
    //Set up lock, allows one thread in critical section at a time
    void * t = NO_CALLBACK;
    db_execute(compose_init_tables(), &t);

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

//Validity checking done inside
int db_create_user(char * name, char * pass) {
    char * password = malloc(50);
    strcpy(password, pass);
    //Aquire the create lock
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;
    semop(createsem, &sb, 1);

    //printf("Have create lock\n");
    if( db_user_exists(name) ) {
        //Release create lock
        sb.sem_op = 1;
        semop(createsem, &sb, 1);
        return CREAT_FAILED;
    }

    //Now we aquire writing lock since we are writing
    //No more readers may enter critical section
    
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

    //Wait for all readers to be clear
    //Now we can write, we are the only one here

    void * t = (void *) NO_CALLBACK;
    db_execute(compose_add_user(name, password), &t);
    //Release the lock
    sb.sem_op = 1;
    semop(createsem, &sb, 1);

    sbT.sem_op = 1;
    semop(writesem, &sbT, 1);

    return CREAT_SUCC;
}
int db_validate_user(char * name, char * password) {
    int * type = malloc(sizeof(int));
    *type = VALIDATE_USER_CALLBACK;
    db_execute(compose_validate_user(name, password), (void **) &type);
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
    //Add to reading semaphore
    semop(readsem, &sbR, 1);

    sbT.sem_op = 1;
    semop(writesem, &sbT, 1);
    int * type = malloc(sizeof(int));
    *type = U_EXISTS_CALLBACK;
    db_execute(compose_user_exists(name), (void **) &type);

    sbR.sem_op = -1;
    semop(readsem, &sbR, 1);
    return (*(int *) type == USER_EXISTS);
}
db_game_data_wr * db_games_in_progress(char * name) {
    struct sembuf sbT;
    sbT.sem_num = 0;
    sbT.sem_flg = SEM_UNDO;
    sbT.sem_op = -1;

    //Wait for no writer to have writing lock
    semop(writesem, &sbT, 1);

    gip_hold = malloc(sizeof(db_game_data_wr));
    gip_hold->number_of_games = 0;

    //Type is sent with information about whose callback routine to call
    int * type = malloc(sizeof(int));
    *type = GET_GAMES_IN_PROGRESS_CALLBACK;
    db_execute(compose_get_games_of(name), (void **) &type);

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
    //Add to reading semaphore
    semop(readsem, &sbR, 1);

    sbT.sem_op = 1;
    semop(writesem, &sbT, 1);

    int * type = malloc(sizeof(int));
    *type = GET_KEY_CALLBACK;
    db_execute(compose_get_game_info(name, opponent), (void **) &type);

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
    //Add to reading semaphore
    semop(readsem, &sbR, 1);

    sbT.sem_op = 1;
    semop(writesem, &sbT, 1);

    int * type = malloc(sizeof(int));
    *type = GET_TURN_CALLBACK;
    db_execute(compose_get_game_info(name, opponent), (void *) &type);

    sbR.sem_op = -1;
    semop(readsem, &sbR, 1);

    return ( *(int *) type == my_turn);

}
void db_update_game( struct cli_upload_game * gd, int update_turn) {
    char u1[50];
    char u2[50];

    if( strcmp(gd->name, gd->opponent) < 0 ) {
        strcpy(u1, gd->name);
        strcpy(u2, gd->opponent);
    } else {
        strcpy(u2, gd->name);
        strcpy(u1, gd->opponent);
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
        int * type = malloc(sizeof(int));
        *type = GAME_INFO_CALLBACK; 
        db_execute(compose_get_game_info(u1, u2), (void **) &type);

        if( !update_turn ) {
            //gi is the last player
            if( gi.turn == U2_TURN ){
                if( gi.dist > gd->dist ){
                    gi.u2wins++;
                } else if (gd->dist > gi.dist) {
                    gi.u1wins++;
                }
            } else {
                if( gi.dist > gd->dist ){
                    gi.u1wins++;
                } else if(gd->dist > gi.dist) {
                    gi.u2wins++;
                }
            }
        }
        else {
            gi.turn = !gi.turn; 
        }
        printf("Hereyolo\n");
        void * t = malloc(sizeof(int));
        *(int *) t = NO_CALLBACK;
        db_execute(compose_update_wins(u1, u2, gi.u1wins, gi.u2wins), &t);
        *(int *) t = NO_CALLBACK;
        db_execute(compose_add_challenge(u1, u2, gd->dist, gi.turn, gd->key), &t);

        sbT.sem_op = 1;
        semop(writesem, &sbT, 1);

    } else {
        //Create game
        db_create_game(gd);
    }
}

int db_game_exists(char * a, char * b) {
    int * type = malloc(sizeof(int));
    *type = GAME_EXISTS_CALLBACK; 
    db_execute(compose_game_exists(a, b), (void **) &type);
    return ( *(int *) type == GAME_EXISTS);
}

void db_create_game( cli_upload_game * gd ) {
    int * type = malloc(sizeof(int));
    *type = NO_CALLBACK; 
    db_execute(compose_new_game_entry(gd), (void **) &type);
}   
