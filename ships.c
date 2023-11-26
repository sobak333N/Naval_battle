
#define errExit(msg)    do { perror(msg); _exit(EXIT_FAILURE); } while (0)

#define BUF 2048
#define BUF_UNIX 100


#define START_UNIX_SOCK_PATH "/tmp/unix_start1.server"
#define HANDLER_MOVE_UNIX_SOCK_PATH1 "/tmp/unix_handler_move1.server"
#define HANDLER_MOVE_UNIX_SOCK_PATH2 "/tmp/unix_handler_move2.server"

#define SERVER_PATH "/tmp/unix_chat.server"
#define CLIENT_PATH "unix_sock.client"
#define FIN "exit"
#define GET "GET"
#define DATA "Answer\n"
#include "data.h"


void child_handler(int sig) {
    pid_t pid;
    while((pid = waitpid(-1, NULL, WNOHANG)) > 0);
}

// int newsession()

struct info{
    int inGame;
    int left;
    int myShips[10][10];
    int myShots[10][10];
    int myShips_info[10][5];
    int myTurn;
};

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



void session(int msgsock,int arr[],int counter,char* names[],char ibuf[],char* name,struct info arr_info[], int wfd,int mat_inc[14][14]){

  dprintf(1,"%d start SESSION\n",msgsock);
    int n;   
   int sock;
   socklen_t length;
   struct sockaddr_in server;
   char obuf[BUF];
   int rval,sval;
   const int enable = 1; 

      if(strncmp(ibuf,"GET",3)==0){
         struct stat filestat;
         int f;
         int i;
        if ((f = open("welcome", O_RDONLY)) == -1) {
                if ((f = open("error", O_RDONLY)) == -1){
                    perror("open: ");
                    exit(errno);
                }
            }
         fstat(f , &filestat);
         int size = filestat.st_size;
         char* try;
         try = (char *)mmap(0,size , PROT_READ , MAP_SHARED,f,0);
         sval=send(msgsock,try,strlen(try),0);
         int status = 2;
         write(wfd,&status,sizeof(int));
      }
      else if(strncmp(ibuf,"POST",4)==0){
            struct stat filestat;
            int f;
            int i;
            for(i = strlen(ibuf)-1 ; ibuf[i]!='=' ;i--){}
            char* mes = malloc(sizeof(char)*(strlen(ibuf)-i));
            int c = 0;
            int tmpI = i;
            i++;
            while(i<strlen(ibuf)){mes[c]=ibuf[i];c++;i++;}
            dprintf(1,"mes[%s]\n",mes);

            tmpI = tmpI-4;
            for(i = tmpI; ibuf[i]!='=' ;i--){}
            char* rec = malloc(sizeof(char)*(tmpI-i+1));
            c = 0;
            i++;
            while(i<tmpI){rec[c+1]=ibuf[i];c++;i++;}
            rec[0] = '/';
            dprintf(1,"rec[%s]\n",rec);


            for(c = 0 ; c < counter ; c++){
                if(strcmp(names[c],rec)==0){break;}
            }
            int c2;
            for(c2 = 0 ; c2 < counter ; c2++){
                if(strcmp(names[c2],name)==0){break;}
            }

            if(c == counter){
                if ((f = open("nouser", O_RDONLY)) == -1) {
                    if ((f = open("error", O_RDONLY)) == -1){
                        perror("open: ");
                        exit(errno);
                    }
                }
                fstat(f , &filestat);
                int size = filestat.st_size;
                char* try;
                try = (char *)mmap(0,size , PROT_READ , MAP_SHARED,f,0);
                sval=send(msgsock,try,strlen(try),0);
                int status = 2;
                write(wfd,&status,sizeof(int));
            }
            else{
                if(strcmp(mes,"start")==0){
                    gen_field(arr_info[c].myShips,arr_info[c].myShips_info,0);
                    generate_shots(arr_info[c].myShots);
                    print_matrix(arr_info[c].myShips);
                    print_matrix(arr_info[c].myShots);

                    gen_field(arr_info[c2].myShips,arr_info[c2].myShips_info,1);
                    generate_shots(arr_info[c2].myShots);
                    print_matrix(arr_info[c2].myShips);
                    print_matrix(arr_info[c2].myShots);
                    static char *newargv[] = {NULL, NULL, NULL};
                    static char *newenviron[] = {NULL};
                    static char *cmd = "/usr/bin/python3";
                    pid_t pid;

                    newargv[0] = "python3";
                    newargv[1] = "start.py";

                    if((pid = fork() ) == -1){perror("fork");exit(1);}
                    int status_child;
                    if(pid == 0){
                        if((status_child=execve(cmd,newargv,newenviron))==-1){perror("execve");exit(1);};
                        exit(0);
                    }
                    int client_sock, rc, len;
                    struct sockaddr_un server_sockaddr; 
                    struct sockaddr_un client_sockaddr; 
                    char buf[BUF_UNIX];
                    size_t bs=BUF_UNIX;
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
                    strcpy(server_sockaddr.sun_path, START_UNIX_SOCK_PATH);
                    rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
                    
                    while(rc == -1){
                        rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
                    }
                    
                    rc = send(client_sock, name, strlen(name), 0);
                    rc = send(client_sock, " ", 1, 0);
                    for(int i = 0 ; i < 10 ; i++){
                        for(int j = 0 ; j < 10 ; j++){
                            if(arr_info[c].myShips[i][j]==0)rc = send(client_sock, "0" , 1 , 0);
                            else rc = send(client_sock, "1" , 1 , 0);
                        }
                        for(int j = 0 ; j < 10 ; j++){
                            if(arr_info[c].myShots[i][j]==0)rc = send(client_sock, "0" , 1 , 0);
                            else rc = send(client_sock, "1" , 1 , 0);
                        }
                    }
                    rc = send(client_sock, "F" , 1 , 0);
                    rc = recv(client_sock, obuf, BUF,0);
                    sval=send(arr[c],obuf,strlen(obuf),0);


                    memset(obuf,0,BUF);

                    close(client_sock);
                    int client_sock2, len2;
                    struct sockaddr_un server_sockaddr2; 
                    struct sockaddr_un client_sockaddr2; 
                    char buf2[BUF_UNIX];
                    size_t bs2=BUF_UNIX;
                    char cpath2[16];
                    newargv[0] = "python3";
                    newargv[1] = "start.py";


                    if((pid = fork() ) == -1){perror("fork");exit(1);}
                    if(pid == 0){
                        if((status_child=execve(cmd,newargv,newenviron))==-1){perror("execve");exit(1);};
                    }
                    len2 = sizeof(client_sockaddr2);
                    memset(&server_sockaddr2, 0, sizeof(struct sockaddr_un));
                    memset(&client_sockaddr2, 0, sizeof(struct sockaddr_un));
                    
                    client_sock2 = socket(AF_UNIX, SOCK_STREAM, 0);
                    if (client_sock2 == -1) {
                        perror("socket");
                        exit(1);
                    }


                    client_sockaddr2.sun_family = AF_UNIX;   
                    strcpy(client_sockaddr2.sun_path, cpath2); 
                    
                    unlink(cpath2);
                    rc = bind(client_sock2, (struct sockaddr *) &client_sockaddr2, len2);
                    if (rc == -1){
                        perror("bind");
                        close(client_sock2);
                        exit(1);
                    }
                        
                    server_sockaddr2.sun_family = AF_UNIX;
                    strcpy(server_sockaddr2.sun_path, START_UNIX_SOCK_PATH);
                    rc = connect(client_sock2, (struct sockaddr *) &server_sockaddr2, len2);
                    
                    while(rc == -1){
                        rc = connect(client_sock2, (struct sockaddr *) &server_sockaddr2, len2);
                    }

                    rc = send(client_sock2, rec, strlen(rec), 0);
                    rc = send(client_sock2, " ", 1, 0);
                    for(int i = 0 ; i < 10 ; i++){
                        for(int j = 0 ; j < 10 ; j++){
                            if(arr_info[c2].myShips[i][j]==0)rc = send(client_sock2, "0" , 1 , 0);
                            else rc = send(client_sock2, "1" , 1 , 0);
                        }
                        for(int j = 0 ; j < 10 ; j++){
                            if(arr_info[c2].myShots[i][j]==0)rc = send(client_sock2, "0" , 1 , 0);
                            else rc = send(client_sock2, "1" , 1 , 0);
                        }
                    }
                    rc = send(client_sock2, "F" , 1 , 0);
                    rc = recv(client_sock2, obuf, BUF,0);
                    close(client_sock2);
                    dprintf(1,"%s",obuf);

                    if ((f = open("send", O_RDONLY)) == -1) {
                        if ((f = open("error", O_RDONLY)) == -1){
                            perror("open: ");
                            exit(errno);
                        }
                    }
                    fstat(f , &filestat);
                    int size = filestat.st_size;
                    char* try;
                    try = (char *)mmap(0,size , PROT_READ , MAP_SHARED,f,0);
                    sval=send(msgsock,try,strlen(try),0);
                    sval=send(msgsock,obuf,strlen(obuf),0);
                    int w;
                    int status = 3;
                    w = write(wfd,&status,sizeof(int));dprintf(1,"w = %d ",w);
                    for(int i = 0 ; i < 10 ; i++){
                        for(int j = 0 ; j < 5 ; j++){
                            w = write(wfd,&arr_info[c2].myShips_info[i][j],sizeof(int));dprintf(1,"w = %d ",w);
                        }
                    }
                    w = write(wfd,&c,sizeof(int));dprintf(1,"w = %d ",w);
                    for(int i = 0 ; i < 10 ; i++){
                        for(int j = 0 ; j < 5 ; j++){
                            w = write(wfd,&arr_info[c].myShips_info[i][j],sizeof(int));dprintf(1,"w = %d ",w);
                        }
                    }
                }
                // HANDLING MOVE
                else if(strlen(mes) == 3 && mes[0]=='m' && mes[1]>=48 && mes[2]>=48 && mes[1]<=57 && mes[1]<=57){
                    int end = 0;
                    int kill = 0;
                    int flag1 = 0 , flag2 = 0, flag3 = 0;
                    if(mat_inc[c][c2]==0){
                        flag1 = 1;
                        //u not play with this player
                        // errexit
                    }
                    if(!flag1 && arr_info[c2].myShots[mes[1]-48][mes[2]-48] > 0){
                        flag2 = 1;
                        // u allready shoot in this point
                        // errexit
                    }
                    if(!flag1 && !flag2 && arr_info[c2].myTurn == 0){
                        flag3 = 1;
                        // not ur turn !
                        //errexit
                    }
                    int i=-1;
                    if(!(flag1+flag2+flag3) && arr_info[c].myShips[mes[1]-48][mes[2]-48] == 1){
                        arr_info[c2].myShots[mes[1]-48][mes[2]-48] = 3;
                        arr_info[c].myShips[mes[1]-48][mes[2]-48] = 3;
                        for(i = 0 ; i < 10 ; i++){
                            if(arr_info[c].myShips_info[i][0]){
                                if(mes[2]-48==arr_info[c].myShips_info[i][1] && mes[1]-48 >= arr_info[c].myShips_info[i][2] && mes[1]-48 <= arr_info[c].myShips_info[i][3]){
                                    arr_info[c].myShips_info[i][4]--;
                                    if(arr_info[c].myShips_info[i][4] == 0){
                                        arr_info[c].left--;
                                        kill =1;
                                    }
                                    if(arr_info[c].left==0)end = 1;
                                    break;
                                }
                            }
                            else{
                                if(mes[1]-48==arr_info[c].myShips_info[i][1] && mes[2]-48 >= arr_info[c].myShips_info[i][2] && mes[2]-48 <= arr_info[c].myShips_info[i][3]){
                                    arr_info[c].myShips_info[i][4]--;
                                    if(arr_info[c].myShips_info[i][4] == 0){
                                        arr_info[c].left--;
                                        kill =1;
                                    }
                                    if(arr_info[c].left==0)end = 1;
                                    break;
                                }
                            }
                        }
                        
                    }
                    if(!(flag1+flag2+flag3) && arr_info[c].myShips[mes[1]-48][mes[2]-48] == 0){
                        arr_info[c2].myShots[mes[1]-48][mes[2]-48] = 2;
                        arr_info[c].myShips[mes[1]-48][mes[2]-48] = 2;
                    }

                    static char *newargv[] = {NULL, NULL, NULL};
                    static char *newenviron[] = {NULL};
                    static char *cmd = "/usr/bin/python3";
                    pid_t pid;
                    int rc;
                    newargv[0] = "python3";
                    newargv[1] = "handler_move.py";

                    if(!(flag1+flag2+flag3)){
                        memset(obuf,0,BUF);
                        if((pid = fork() ) == -1){perror("fork");exit(1);}
                        int status_child;
                        if(pid == 0){
                            if((status_child=execve(cmd,newargv,newenviron))==-1){perror("execve");exit(1);};
                            exit(0);
                        }
                        int client_sock, len;
                        struct sockaddr_un server_sockaddr; 
                        struct sockaddr_un client_sockaddr; 
                        char buf[BUF_UNIX];
                        size_t bs=BUF_UNIX;
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
                        strcpy(server_sockaddr.sun_path, HANDLER_MOVE_UNIX_SOCK_PATH1);
                        rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
                        
                        while(rc == -1){
                            rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
                        }
                        
                        rc = send(client_sock, name, strlen(name), 0);
                        rc = send(client_sock, " ", 1, 0);
                        for(int i = 0 ; i < 10 ; i++){
                            for(int j = 0 ; j < 10 ; j++){
                                if(arr_info[c].myShips[i][j]==0)rc = send(client_sock, "0" , 1 , 0);
                                else if(arr_info[c].myShips[i][j]==1)rc = send(client_sock, "1" , 1 , 0);
                                else if(arr_info[c].myShips[i][j]==2)rc = send(client_sock, "2" , 1 , 0);
                                else if(arr_info[c].myShips[i][j]==3)rc = send(client_sock, "3" , 1 , 0);
                            }
                            for(int j = 0 ; j < 10 ; j++){
                                if(arr_info[c].myShots[i][j]==0)rc = send(client_sock, "0" , 1 , 0);
                                else if(arr_info[c].myShots[i][j]==2)rc = send(client_sock, "2" , 1 , 0);
                                else if(arr_info[c].myShots[i][j]==3)rc = send(client_sock, "3" , 1 , 0);
                            }
                        }
                        if(end)rc = send(client_sock, "L" , 1 , 0);
                        rc = send(client_sock, "F" , 1 , 0);
                        rc = recv(client_sock, obuf, BUF,0);
                        sval=send(arr[c],obuf,strlen(obuf),0);
                        close(client_sock);
                    }
                    memset(obuf,0,BUF);
                    int client_sock2, len2,status_child;
                    struct sockaddr_un server_sockaddr2; 
                    struct sockaddr_un client_sockaddr2; 
                    char buf2[BUF_UNIX];
                    size_t bs2=BUF_UNIX;
                    char cpath2[16];
                    newargv[0] = "python3";
                    newargv[1] = "handler_move2.py";


                    if((pid = fork() ) == -1){perror("fork");exit(1);}
                    if(pid == 0){
                        if((status_child=execve(cmd,newargv,newenviron))==-1){perror("execve");exit(1);};
                    }
                    len2 = sizeof(client_sockaddr2);
                    memset(&server_sockaddr2, 0, sizeof(struct sockaddr_un));
                    memset(&client_sockaddr2, 0, sizeof(struct sockaddr_un));
                    
                    client_sock2 = socket(AF_UNIX, SOCK_STREAM, 0);
                    if (client_sock2 == -1) {
                        perror("socket");
                        exit(1);
                    }


                    client_sockaddr2.sun_family = AF_UNIX;   
                    strcpy(client_sockaddr2.sun_path, cpath2); 
                    
                    unlink(cpath2);
                    rc = bind(client_sock2, (struct sockaddr *) &client_sockaddr2, len2);
                    if (rc == -1){
                        perror("bind");
                        close(client_sock2);
                        exit(1);
                    }
                        
                    server_sockaddr2.sun_family = AF_UNIX;
                    strcpy(server_sockaddr2.sun_path, HANDLER_MOVE_UNIX_SOCK_PATH2);
                    rc = connect(client_sock2, (struct sockaddr *) &server_sockaddr2, len2);
                    
                    while(rc == -1){
                        rc = connect(client_sock2, (struct sockaddr *) &server_sockaddr2, len2);
                    }

                    rc = send(client_sock2, rec, strlen(rec), 0);
                    rc = send(client_sock2, " ", 1, 0);
                    for(int i = 0 ; i < 10 ; i++){
                        for(int j = 0 ; j < 10 ; j++){
                            if(arr_info[c2].myShips[i][j]==0)rc = send(client_sock2, "0" , 1 , 0);
                            else if(arr_info[c2].myShips[i][j]==1)rc = send(client_sock2, "1" , 1 , 0);
                            else if(arr_info[c2].myShips[i][j]==2)rc = send(client_sock2, "2" , 1 , 0);
                            else if(arr_info[c2].myShips[i][j]==3)rc = send(client_sock2, "3" , 1 , 0);
                        }
                        for(int j = 0 ; j < 10 ; j++){
                            if(arr_info[c2].myShots[i][j]==0)rc = send(client_sock2, "0" , 1 , 0);
                            else if(arr_info[c2].myShots[i][j]==2)rc = send(client_sock2, "2" , 1 , 0);
                            else if(arr_info[c2].myShots[i][j]==3)rc = send(client_sock2, "3" , 1 , 0);
                        }
                    }
                    if(flag1)rc = send(client_sock2, "A" , 1 , 0);
                    if(flag2)rc = send(client_sock2, "B" , 1 , 0);
                    if(flag3)rc = send(client_sock2, "C" , 1 , 0);
                    if(kill)rc = send(client_sock2, "K" , 1 , 0);
                    if(end)rc = send(client_sock2, "E" , 1 , 0);
                    rc = send(client_sock2, "F" , 1 , 0);
                    rc = recv(client_sock2, obuf, BUF,0);
                    close(client_sock2);
                    dprintf(1,"%s",obuf);

                    if ((f = open("send", O_RDONLY)) == -1) {
                        if ((f = open("error", O_RDONLY)) == -1){
                            perror("open: ");
                            exit(errno);
                        }
                    }
                    fstat(f , &filestat);
                    int size = filestat.st_size;
                    char* try;
                    try = (char *)mmap(0,size , PROT_READ , MAP_SHARED,f,0);

                    sval=send(msgsock,try,strlen(try),0);
                    sval=send(msgsock,obuf,strlen(obuf),0);
                    int w;
                    int status;
                    if(!(flag1+flag2+flag3)){
                        status = 1;
                        int x = mes[1]-48;
                        int y = mes[2]-48;
                        w = write(wfd,&status,sizeof(int));
                        w = write(wfd,&c,sizeof(int));
                        w = write(wfd,&x,sizeof(int));
                        w = write(wfd,&y,sizeof(int));
                        w = write(wfd,&i,sizeof(int));
                        int k = kill==0 ? 0 : 1;
                        w = write(wfd,&k,sizeof(int)); 
                    }
                    else{
                        status = 2;
                        w = write(wfd,&status,sizeof(int));
                    }
                }
                // JUST MESSAGES
                else{
                static char *newargv[] = {NULL, NULL, NULL};
                static char *newenviron[] = {NULL};
                static char *cmd = "/usr/bin/python3";
                pid_t pid;

                newargv[0] = "python3";
                newargv[1] = "handler_chat.py";

                if((pid = fork() ) == -1){perror("fork");exit(1);}
                int status_child;
                if(pid == 0){
                    if((status_child=execve(cmd,newargv,newenviron))==-1){perror("execve");exit(1);};
                }



                int client_sock, rc, len;
                struct sockaddr_un server_sockaddr; 
                struct sockaddr_un client_sockaddr; 
                char buf[BUF_UNIX];
                size_t bs=BUF_UNIX;
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
                strcpy(server_sockaddr.sun_path, SERVER_PATH);
                rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
                while(rc == -1){
                rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
                }
                rc = send(client_sock, mes, strlen(mes), 0);
                rc = send(client_sock, name, strlen(name), 0);
                rc = recv(client_sock, obuf, BUF,0);
                close(client_sock);
                dprintf(1,"%s\n",obuf);
                dprintf(1,"%s %d\n",names[c],arr[c]);
                sval=send(arr[c],obuf,strlen(obuf),0);

                if ((f = open("send", O_RDONLY)) == -1) {
                        if ((f = open("error", O_RDONLY)) == -1){
                            perror("open: ");
                            exit(errno);
                        }
                    }
                fstat(f , &filestat);
                int size = filestat.st_size;
                char* try;
                try = (char *)mmap(0,size , PROT_READ , MAP_SHARED,f,0);
                sval=send(msgsock,try,strlen(try),0);
                // dprintf(1,"send_to_sender[%d]",sval);
                int status = 2;
                write(wfd,&status,sizeof(int));
                }
                
            }
            
      }
//    } while ((rval > 0) && (strncmp(ibuf,GET,3)));
    close(wfd);
    printf("session DIEEEEEEEEEE \n");
   exit(0);
}


int main(int argc, char *argv[]) {
   int sock;
   socklen_t length;
   struct sockaddr_in server;
   int msgsock;
   char ibuf[BUF];
   char obuf[BUF];
   int rval,sval;
   const int enable = 1; 


   struct sigaction new_act, old_act;
   sigemptyset (&new_act.sa_mask);
   new_act.sa_flags = SA_RESTART;
   new_act.sa_handler = child_handler;
   sigaction (SIGCHLD, &new_act, &old_act);

   sock = socket(AF_INET, SOCK_STREAM, 0);
   if (sock == -1) {
      perror("opening stream socket");
      exit(1);
   }
   if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) exit(2);
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = htonl(INADDR_ANY);
   if(argc == 2) server.sin_port = htons(atoi(argv[1]));
   else server.sin_port = 0;
   
   if (bind(sock, (struct sockaddr *) &server, sizeof server)  == -1) {
      perror("binding stream socket");
      exit(1);
   }

   length = sizeof server;
   if (getsockname(sock,(struct sockaddr *) &server, &length) == -1) {
      perror("getting socket name");
      exit(1);
   }
   printf("Socket port %d\n", ntohs(server.sin_port));

   listen(sock, 5);
    pid_t cpid;


    int arr[] ={11,12,13,14,15,16,17,18,19,20,21,22,23,24};
    int counter = 0;
    char *names[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    int mat_inc[14][14] = {0};
    struct info arr_info[14];
    int pipe_fd[14][2];
   
    int fd;
    int rd;
    int buf_pipe[500];
    memset(buf_pipe,0,sizeof(buf_pipe));

   do {
      if ((msgsock = accept(sock,(struct sockaddr *) NULL,(socklen_t *) NULL)) == -1) 
         perror("accept");
      else{
         int i;
         char ibuf[BUF];
         char* name;
         memset(ibuf, 0, BUF);
         if ((rval = read(msgsock, ibuf, 1024)) == -1){
            perror("reading stream message");
         }


         if(strncmp(ibuf,"POST",4)==0){
            dprintf(1,"[%s]\n",ibuf);
            for(i = 5 ; ibuf[i] != ' ' ; i++){
            }
            name = malloc(sizeof(char)*(i-5));
            
            for(i = 5 ; ibuf[i] != ' ' ; i++){
                name[i-5] = ibuf[i];
            }
            dprintf(1,"name[%s]\n",name);
         }

         else if(strncmp(ibuf,"GET",3)==0){
            dprintf(1,"[%s]\n",ibuf);
            for(i = 4 ; ibuf[i] != ' ' ; i++){
            }
            name = malloc(sizeof(char)*(i-4));
            
            for(i = 4 ; ibuf[i] != ' ' ; i++){
                name[i-4] = ibuf[i];
            }
            dprintf(1,"name[%s]\n",name);
         }

        
        for(i = 0 ; i < counter ; i++){
            dprintf(1,"%s ",names[i]);
            if(strcmp(names[i],name)==0){
                dup2(msgsock, arr[i]);
                close(msgsock);
                cpid = fork();
                // pipe(pipe_fd[i]);
                if(cpid == 0){
                    dprintf(1,"%d start SESSION\n",arr[i]);
                    session(arr[i],arr,counter,names,ibuf,name,arr_info,pipe_fd[i][1],mat_inc);
                }
                break;
            }
        }
        if(i == counter){
            dprintf(1,"accept con ");
            dup2(msgsock, arr[counter]);
            close(msgsock);
            dprintf(1,"msgsock = %d \n",arr[counter]);
            pipe(pipe_fd[counter]);
            cpid = fork();
            if(cpid == 0){
                close(pipe_fd[counter][0]);
                session(arr[counter],arr,counter,names,ibuf,name,arr_info,pipe_fd[counter][1],mat_inc);
            }
        }

        int flag = 0;
        for(int j = 0 ; j < counter ; j++){
            dprintf(1,"%s ",names[i]);
            if(strcmp(names[j],name)==0){flag=1;dprintf(1,"%s %s\n",names[j],name);}
        }
        if(!flag){
            names[counter] = malloc(strlen(name)+1);
            strcpy(names[counter],name);
            counter++;
        }
        waitpid(cpid,NULL,0);
        int r = read(pipe_fd[i][0],&buf_pipe[0],sizeof(int));
        dprintf(1,"r = %d buf_pipe[0] = %d " , r , buf_pipe[0]);
        if(r != -1 && buf_pipe[0]==3){
            for(int j = 1 ; j < 102 ; j++){
                r = read(pipe_fd[i][0],&buf_pipe[j],sizeof(int));
                dprintf(1,"r%d ",r);
            }
            dprintf(1," end reading ");
            for(int j = 0 ; j < 102 ; j++){
                dprintf(1,"%d ",buf_pipe[j]);
            }
            for(int j = 1 ; j < 51 ; j++){
                arr_info[i].myShips_info[(j-1)/5][(j-1)%5] = buf_pipe[j];
            }
            int another = buf_pipe[51];
            for(int j = 52 ; j < 102 ; j++){
                arr_info[another].myShips_info[(j-52)/5][(j-52)%5] = buf_pipe[j];
            }
            for(int j = 0 ; j < 10 ; j++)feel_one(arr_info[i].myShips , j , arr_info[i].myShips_info);
            for(int j = 0 ; j < 10 ; j++)feel_one(arr_info[another].myShips , j , arr_info[another].myShips_info);
            memset(arr_info[i].myShots,0,sizeof(arr_info[i].myShots));
            memset(arr_info[another].myShots,0,sizeof(arr_info[another].myShots));
            print_matrix(arr_info[i].myShips);
            print_matrix(arr_info[i].myShots);
            print_matrix(arr_info[another].myShips);
            print_matrix(arr_info[another].myShots);
            mat_inc[i][another]=1;
            mat_inc[another][i]=1;
            arr_info[another].myTurn=1;
            arr_info[i].myTurn=0;
            arr_info[another].left = 10;
            arr_info[i].left = 10;
        }
        if(r!=-1 && buf_pipe[0]==1){
            for(int j = 1 ; j < 6 ; j++){
                r = read(pipe_fd[i][0],&buf_pipe[j],sizeof(int));
            }
            int another = buf_pipe[1];
            int x = buf_pipe[2];
            int y = buf_pipe[3];
            int sh  = buf_pipe[4];
            int k = buf_pipe[5];
            if(k == 1)arr_info[another].left--;
            if(sh != -1){
                arr_info[another].myShips_info[sh][4]--;
                arr_info[another].myShips[x][y] = 3;
                arr_info[i].myShots[x][y] = 3;
            }
            else{
                arr_info[another].myShips[x][y] = 2;
                arr_info[i].myShots[x][y] = 2;
            }
            arr_info[i].myTurn=0;
            arr_info[another].myTurn=1;
        }

        } 
    memset(buf_pipe,0,sizeof(buf_pipe));
   } while(strcmp(ibuf,FIN));

   exit(0);
}