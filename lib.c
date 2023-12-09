#include "data.h"

void generate_shots(int shots[10][10]){
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0 ; j < 10 ;j++){
            shots[i][j]=0;
        }
    }
}

void print_matrix(int mat[10][10]){
    printf("\n");
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0 ; j < 10 ;j++){
            // printf("%d ",mat[i][j]);
            if(mat[i][j]==1)dprintf(1,"#");
            if(mat[i][j]==0)dprintf(1,"_");
        }
        printf("\n");
    }
    printf("--------------------------\n");
}


void feel_two_xd(int field[10][10] , int j , int ships[10][5]){
    if(ships[j][1]>0){
        int high = ships[j][3] < 9 ? ships[j][3]+2 : 10 ;
        for(int i = ships[j][2] > 0 ? ships[j][2]-1 : 0 ; i < high; i++){
            if(ships[j][0]){
                field[i][ships[j][1]-1] = 2;
            }
            else{
                field[ships[j][1]-1][i] = 2;
            }
        }
    }
    if(ships[j][1]<9){
        int high = ships[j][3] < 9 ? ships[j][3]+2 : 10 ;
        for(int i = ships[j][2] > 0 ? ships[j][2]-1 : 0 ; i < high; i++){
            // printf("%d %d\n",i,ships[j][1]-1);
            if(ships[j][0]){
                field[i][ships[j][1]+1] = 2;
            }
            else{
                field[ships[j][1]+1][i] = 2;
            }
        }
    }
    if(ships[j][2]>0){
        if(ships[j][0]){
            field[ships[j][2]-1][ships[j][1]]=2;
        }
        else{
            field[ships[j][1]][ships[j][2]-1]=2;
        }
    }
    if(ships[j][3]<9){
        if(ships[j][0]){
            field[ships[j][3]+1][ships[j][1]]=2;
        }
        else{
            field[ships[j][1]][ships[j][3]+1]=2;
        }
    }
}

void rand_pos(int ships[10][5] , int num){
    int offset;
    if(num == 0)offset = 4;
    if(num >= 1 && num <=2)offset = 3;
    if(num >= 3 && num <= 5)offset =2;
    if(num >= 6)offset = 1;
    ships[num][1] = rand()%10;
    ships[num][2] = rand()%(10-offset+1);
    ships[num][3] = ships[num][2]+offset-1;
    ships[num][4] = offset;
}

void feel_one(int field[10][10], int j , int ships[10][5]){
    for(int i = ships[j][2] ; i < ships[j][3]+1 ; i++){
        if(ships[j][4]){
            if(ships[j][0]){
                field[i][ships[j][1]] = 1;
            }
            else{
                field[ships[j][1]][i] = 1;
            }
        }
    }
}

void gen_field(int field[10][10],int ships[10][5],int t){
    // int ships[10][5];
    srand(getpid()+t);
    for(int i = 0 ; i < 10 ; i++){
        ships[i][0] = rand()%2;
    }

    // rand_pos(ships,0);
    // feel_one(field,0,ships);
    // print_field(field);
    // feel_two_xd(field,0,ships);
    // print_field(field);


    for(int i = 0; i < 10 ; i++){
        rand_pos(ships,i);
        if(ships[i][0]){
            while(field[ships[i][2]][ships[i][1]]>0 || field[ships[i][3]][ships[i][1]]>0 ){
                rand_pos(ships,i);
                // printf("%d %d \n",field[ships[1][2]][ships[1][1]],field[ships[1][3]][ships[1][1]]);
            }
        }
        else{
            while(field[ships[i][1]][ships[i][2]]>0 || field[ships[i][1]][ships[i][3]]>0 ){
                rand_pos(ships,i);
            }
        }
        feel_one(field,i,ships);
        // print_field(field);
        feel_two_xd(field,i,ships);
        // print_field(field);
    }
    // print_field(field);
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0; j < 10 ;j++){
            if(field[i][j]==2)field[i][j]=0;
        }
    }
}


void tmp_user(struct user_info* user, int offset , int* bd){
    user->inGame = bd[offset*PERSON_INFO_SIZE];
    user->op = bd[offset*PERSON_INFO_SIZE+1];
    user->myTurn = bd[offset*PERSON_INFO_SIZE+2];
    user->left = bd[offset*PERSON_INFO_SIZE+3];
    for(int i = 0 ; i < 50 ; i++){
        user->myShips_info[i/5][i%5] = bd[offset*PERSON_INFO_SIZE+4+i];
    }
    for(int i = 0 ; i < 100 ; i++){
        user->myShots[i/10][i%10] = bd[offset*PERSON_INFO_SIZE+54+i];
    }
    for(int i = 0 ; i < 100 ; i++){
        user->myShips[i/10][i%10] = bd[offset*PERSON_INFO_SIZE+154+i];
    }
    printf("inGame=%d op=%d myTurn=%d left=%d\n",user->inGame,user->op,user->myTurn,user->left);
    print_matrix(user->myShips);
    print_matrix(user->myShots);
}

int open_unix_sock(char* name_of_prog){
    static char *newargv[] = {NULL, NULL, NULL};
    static char *newenviron[] = {NULL};
    static char *cmd = "/usr/bin/python3";
    pid_t pid;

    newargv[0] = "python3";
    newargv[1] = name_of_prog;

    if((pid = fork() ) == -1){perror("fork");exit(1);}
    int status_child;
    if(pid == 0){
        if((status_child=execve(cmd,newargv,newenviron))==-1){perror("execve");exit(1);};
        exit(0);
    }
    int client_sock, rc, len;
    struct sockaddr_un server_sockaddr; 
    struct sockaddr_un client_sockaddr; 
    char cpath[16];


    len = sizeof(client_sockaddr);
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    
    client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_sock == -1) {
        perror("socket");
        exit(1);
    }


    client_sockaddr.sun_family = AF_UNIX;   
    strcpy(client_sockaddr.sun_path, cpath); 
    
    unlink(cpath);
    rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
    if (rc == -1){
        perror("bind");
        close(client_sock);
        exit(1);
    }
        
    server_sockaddr.sun_family = AF_UNIX;
    if(strcmp(name_of_prog,"error_handler.py")==0)strcpy(server_sockaddr.sun_path, ERROR_START__UNIX_SOCK_PATH1);
    if(strcmp(name_of_prog,"start.py")==0)strcpy(server_sockaddr.sun_path, START_UNIX_SOCK_PATH);
    if(strcmp(name_of_prog,"handler_move.py")==0)strcpy(server_sockaddr.sun_path, HANDLER_MOVE_UNIX_SOCK_PATH1);
    if(strcmp(name_of_prog,"handler_move2.py")==0)strcpy(server_sockaddr.sun_path, HANDLER_MOVE_UNIX_SOCK_PATH2);
    if(strcmp(name_of_prog,"handler_chat.py")==0)strcpy(server_sockaddr.sun_path, HANDLER_CHAT_UNIX_SOCK_PATH );


    rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
    
    while(rc == -1){
        rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
    }
    return client_sock;
}
