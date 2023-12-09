#include "data.h"
void child_handler(int sig) {
    pid_t pid;
    while((pid = waitpid(-1, NULL, WNOHANG)) > 0);
}

struct info{
    int inGame;
    int left;
    int myShips[10][10];
    int myShots[10][10];
    int myShips_info[10][5];
    int myTurn;
};


void session(int msgsock,int arr[],int counter,char* names[],char ibuf[],char* name,int fd_bd){
    int* bd;
    char obuf[BUF];
    int sval,client_sock,rc;
    if ((bd = (int*)mmap(NULL,MAX_USERS*PERSON_INFO_SIZE*sizeof(int) , PROT_READ|PROT_WRITE, MAP_SHARED, fd_bd, 0)) == (void *)-1) {
        perror("mmap bd: ");
        exit(errno);    
    } 
    for(int i = 0 ; i < MAX_USERS*PERSON_INFO_SIZE ; i++){
        printf("%d ",bd[i]);
    }

    if(strncmp(ibuf,"GET",3)==0){
        struct stat filestat;
        int f;
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
        free(try);
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
            }
            else{
                struct user_info reciever;
                struct user_info sender;
                tmp_user(&reciever,c,bd);
                tmp_user(&sender,c2,bd);
                print_matrix(reciever.myShips);
                print_matrix(reciever.myShots);
                print_matrix(sender.myShips);
                print_matrix(sender.myShots);
                if(strcmp(mes,"start")==0){
                    int flag1 = 0, flag2 = 0,flag3 = 0,flag4 = 0;
                    if(reciever.op==c2 && sender.op == c && (c2!=c && c2!=0)){
                        // u allready play with this player
                        flag1 = 1;
                    }
                    if(!flag1 && reciever.inGame){
                        flag2 = 1;
                        // this player is allready in game with another player . wait for their end
                    }
                    if(!(flag1+flag2) && sender.inGame){
                        flag3 = 1;
                        // u are allready in game with another player
                    }
                    if(!(flag1+flag2+flag3) && c == c2){
                        flag4 = 1;
                    }
                    if(flag1 || flag2 || flag3 || flag4){
                        client_sock = open_unix_sock("error_handler.py");
                        
                        if(flag1)rc = send(client_sock, "A", 1, 0);
                        if(flag2)rc = send(client_sock, "B", 1, 0);
                        if(flag3)rc = send(client_sock, "C", 1, 0);
                        if(flag4)rc = send(client_sock, "D", 1, 0);
                        rc = send(client_sock, "F", 1, 0);
                        memset(obuf,0,BUF);
                        rc = recv(client_sock, obuf, BUF,0);
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
                        sval=send(msgsock,obuf,strlen(obuf),0);
                        close(client_sock);
                        free(try);
                        exit(0);
                    }

                    gen_field(reciever.myShips,reciever.myShips_info,0);
                    memset(reciever.myShots,0,sizeof(reciever.myShots));
                    print_matrix(reciever.myShips);
                    print_matrix(reciever.myShots);


                    gen_field(sender.myShips,sender.myShips_info,1);
                    memset(sender.myShots,0,sizeof(sender.myShots));
                    print_matrix(sender.myShips);
                    print_matrix(sender.myShots);


                    client_sock = open_unix_sock("start.py");
                    rc = send(client_sock, name, strlen(name), 0);
                    rc = send(client_sock, " ", 1, 0);
                    for(int i = 0 ; i < 10 ; i++){
                        for(int j = 0 ; j < 10 ; j++){
                            if(reciever.myShips[i][j]==0)rc = send(client_sock, "0" , 1 , 0);
                            else rc = send(client_sock, "1" , 1 , 0);
                        }
                        for(int j = 0 ; j < 10 ; j++){
                            if(reciever.myShots[i][j]==0)rc = send(client_sock, "0" , 1 , 0);
                            else rc = send(client_sock, "1" , 1 , 0);
                        }
                    }
                    rc = send(client_sock, "F" , 1 , 0);
                    memset(obuf,0,BUF);
                    rc = recv(client_sock, obuf, BUF,0);
                    sval=send(arr[c],obuf,strlen(obuf),0);


                    memset(obuf,0,BUF);

                    close(client_sock);
                    int client_sock2 = open_unix_sock("start.py");

                    rc = send(client_sock2, rec, strlen(rec), 0);
                    rc = send(client_sock2, " ", 1, 0);
                    for(int i = 0 ; i < 10 ; i++){
                        for(int j = 0 ; j < 10 ; j++){
                            if(sender.myShips[i][j]==0)rc = send(client_sock2, "0" , 1 , 0);
                            else rc = send(client_sock2, "1" , 1 , 0);
                        }
                        for(int j = 0 ; j < 10 ; j++){
                            if(sender.myShots[i][j]==0)rc = send(client_sock2, "0" , 1 , 0);
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

                    bd[PERSON_INFO_SIZE*c] = 1;
                    bd[PERSON_INFO_SIZE*c+1] = c2;
                    bd[PERSON_INFO_SIZE*c+2] = 1;
                    bd[PERSON_INFO_SIZE*c+3] = 10;
                    for(int i = 0 ; i < 50 ; i++){
                        bd[PERSON_INFO_SIZE*c+4+i] = reciever.myShips_info[i/5][i%5];
                    }
                    for(int i = 0 ; i < 100 ; i++){
                        bd[PERSON_INFO_SIZE*c+54+i] = reciever.myShots[i/10][i%10];
                    }
                    for(int i = 0 ; i < 100 ; i++){
                        bd[PERSON_INFO_SIZE*c+154+i] = reciever.myShips[i/10][i%10];
                    }

                    bd[PERSON_INFO_SIZE*c2] = 1;
                    bd[PERSON_INFO_SIZE*c2+1] = c;
                    bd[PERSON_INFO_SIZE*c2+2] = 0;
                    bd[PERSON_INFO_SIZE*c2+3] = 10;
                    for(int i = 0 ; i < 50 ; i++){
                        bd[PERSON_INFO_SIZE*c2+4+i] = sender.myShips_info[i/5][i%5];
                    }
                    for(int i = 0 ; i < 100 ; i++){
                        bd[PERSON_INFO_SIZE*c2+54+i] = sender.myShots[i/10][i%10];
                    }
                    for(int i = 0 ; i < 100 ; i++){
                        bd[PERSON_INFO_SIZE*c2+154+i] = sender.myShips[i/10][i%10];
                    }
                    msync(bd, MAX_USERS*PERSON_INFO_SIZE*sizeof(int), MS_SYNC);
                    munmap(bd, MAX_USERS*PERSON_INFO_SIZE*sizeof(int));

                    free(try);
                }
                // HANDLING MOVE
                else if(strlen(mes) == 3 && mes[0]=='m' && mes[1]>=48 && mes[2]>=48 && mes[1]<=57 && mes[1]<=57){
                    int end = 0;
                    int kill = 0;
                    int flag1 = 0 , flag2 = 0, flag3 = 0;
                    if(reciever.op!=c2 || sender.op!=c){
                        flag1 = 1;
                        //u not play with this player
                        // errexit
                    }
                    if(!flag1 && sender.myShots[mes[1]-48][mes[2]-48] > 0){
                        flag2 = 1;
                        // u allready shoot in this point
                        // errexit
                    }
                    if(!flag1 && !flag2 && sender.myTurn == 0){
                        flag3 = 1;
                        // not ur turn !
                        //errexit
                    }
                    int i=-1;
                    if(!(flag1+flag2+flag3) && reciever.myShips[mes[1]-48][mes[2]-48] == 1){
                        sender.myShots[mes[1]-48][mes[2]-48] = 3;
                        reciever.myShips[mes[1]-48][mes[2]-48] = 3;
                        for(i = 0 ; i < 10 ; i++){
                            if(reciever.myShips_info[i][0]){
                                if(mes[2]-48==reciever.myShips_info[i][1] && mes[1]-48 >= reciever.myShips_info[i][2] && mes[1]-48 <= reciever.myShips_info[i][3]){
                                    reciever.myShips_info[i][4]--;
                                    if(reciever.myShips_info[i][4] == 0){
                                        reciever.left--;
                                        kill =1;
                                    }
                                    if(reciever.left==0)end = 1;
                                    break;
                                }
                            }
                            else{
                                if(mes[1]-48==reciever.myShips_info[i][1] && mes[2]-48 >= reciever.myShips_info[i][2] && mes[2]-48 <= reciever.myShips_info[i][3]){
                                    reciever.myShips_info[i][4]--;
                                    if(reciever.myShips_info[i][4] == 0){
                                        reciever.left--;
                                        kill =1;
                                    }
                                    if(reciever.left==0)end = 1;
                                    break;
                                }
                            }
                        }
                        
                    }
                    if(!(flag1+flag2+flag3) && reciever.myShips[mes[1]-48][mes[2]-48] == 0){
                        sender.myShots[mes[1]-48][mes[2]-48] = 2;
                        reciever.myShips[mes[1]-48][mes[2]-48] = 2;
                    }


                    if(!(flag1+flag2+flag3)){
                        client_sock=open_unix_sock("handler_move.py");                              
                        rc = send(client_sock, name, strlen(name), 0);
                        rc = send(client_sock, " ", 1, 0);
                        for(int i = 0 ; i < 10 ; i++){
                            for(int j = 0 ; j < 10 ; j++){
                                if(reciever.myShips[i][j]==0)rc = send(client_sock, "0" , 1 , 0);
                                else if(reciever.myShips[i][j]==1)rc = send(client_sock, "1" , 1 , 0);
                                else if(reciever.myShips[i][j]==2)rc = send(client_sock, "2" , 1 , 0);
                                else if(reciever.myShips[i][j]==3)rc = send(client_sock, "3" , 1 , 0);
                            }
                            for(int j = 0 ; j < 10 ; j++){
                                if(reciever.myShots[i][j]==0)rc = send(client_sock, "0" , 1 , 0);
                                else if(reciever.myShots[i][j]==2)rc = send(client_sock, "2" , 1 , 0);
                                else if(reciever.myShots[i][j]==3)rc = send(client_sock, "3" , 1 , 0);
                            }
                        }
                        if(end)rc = send(client_sock, "L" , 1 , 0);
                        rc = send(client_sock, "F" , 1 , 0);
                        memset(obuf,0,BUF);
                        rc = recv(client_sock, obuf, BUF,0);
                        sval=send(arr[c],obuf,strlen(obuf),0);
                        close(client_sock);
                    }
                    memset(obuf,0,BUF);
                    
                    int client_sock2 = open_unix_sock("handler_move2.py");

                    rc = send(client_sock2, rec, strlen(rec), 0);
                    rc = send(client_sock2, " ", 1, 0);
                    print_matrix(sender.myShips);
                    print_matrix(sender.myShots);
                    for(int i = 0 ; i < 10 ; i++){
                        for(int j = 0 ; j < 10 ; j++){
                            if(sender.myShips[i][j]==0)rc = send(client_sock2, "0" , 1 , 0);
                            else if(sender.myShips[i][j]==1)rc = send(client_sock2, "1" , 1 , 0);
                            else if(sender.myShips[i][j]==2)rc = send(client_sock2, "2" , 1 , 0);
                            else if(sender.myShips[i][j]==3)rc = send(client_sock2, "3" , 1 , 0);
                        }
                        for(int j = 0 ; j < 10 ; j++){
                            if(sender.myShots[i][j]==0)rc = send(client_sock2, "0" , 1 , 0);
                            else if(sender.myShots[i][j]==2)rc = send(client_sock2, "2" , 1 , 0);
                            else if(sender.myShots[i][j]==3)rc = send(client_sock2, "3" , 1 , 0);
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
                    if(end){
                        for(int i = 0 ; i < PERSON_INFO_SIZE ; i++)bd[PERSON_INFO_SIZE*c+i]=0;
                        for(int i = 0 ; i < PERSON_INFO_SIZE ; i++)bd[PERSON_INFO_SIZE*c2+i]=0;
                    }
                    else if(!(flag1+flag2+flag3)){
                        bd[PERSON_INFO_SIZE*c+2] = 1;
                        if(kill)bd[PERSON_INFO_SIZE*c+3]--;
                        for(int i = 0 ; i < 50 ; i++){
                            bd[PERSON_INFO_SIZE*c+4+i] = reciever.myShips_info[i/5][i%5];
                        }
                        for(int i = 0 ; i < 100 ; i++){
                            bd[PERSON_INFO_SIZE*c+54+i] = reciever.myShots[i/10][i%10];
                        }
                        for(int i = 0 ; i < 100 ; i++){
                            bd[PERSON_INFO_SIZE*c+154+i] = reciever.myShips[i/10][i%10];
                        }

                        bd[PERSON_INFO_SIZE*c2+2] = 0;
                        for(int i = 0 ; i < 50 ; i++){
                            bd[PERSON_INFO_SIZE*c2+4+i] = sender.myShips_info[i/5][i%5];
                        }
                        for(int i = 0 ; i < 100 ; i++){
                            bd[PERSON_INFO_SIZE*c2+54+i] = sender.myShots[i/10][i%10];
                        }
                        for(int i = 0 ; i < 100 ; i++){
                            bd[PERSON_INFO_SIZE*c2+154+i] = sender.myShips[i/10][i%10];
                        }
                    }

                    msync(bd, MAX_USERS*PERSON_INFO_SIZE*sizeof(int), MS_SYNC);
                    munmap(bd, MAX_USERS*PERSON_INFO_SIZE*sizeof(int));
                    free(try);
                }
                // JUST MESSAGES
                else{
                    client_sock = open_unix_sock("handler_chat.py");
                    rc = send(client_sock, mes, strlen(mes), 0);
                    rc = send(client_sock, name, strlen(name), 0);
                    memset(obuf,0,BUF);
                    rc = recv(client_sock, obuf, BUF,0);
                    close(client_sock);
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
                    free(try);
                }
                
            }
            
      }

    printf("session DIEEEEEEEEEE \n");
    exit(0);
}


int main(int argc, char *argv[]) {
    printf("start ");
    int sock;
    int* bd;
    socklen_t length;
    struct sockaddr_in server;
    int msgsock;
    int rval;
    int fd_bd;
    const int enable = 1; 
    if ((fd_bd = open(BD, O_RDWR|O_CREAT|O_TRUNC,0664)) == -1) {
        perror("open: ");
        exit(errno);
    }
    lseek(fd_bd, MAX_USERS*PERSON_INFO_SIZE*sizeof(int), SEEK_SET);
    write(fd_bd, "", 1);
    if ((bd = (int*)mmap(NULL,MAX_USERS*PERSON_INFO_SIZE*sizeof(int) , PROT_READ|PROT_WRITE, MAP_SHARED, fd_bd, 0)) == (void *)-1) {
        perror("mmap bd: ");
        exit(errno);    
    }
    memset(bd,0,MAX_USERS*PERSON_INFO_SIZE*sizeof(int));
    msync(bd, MAX_USERS*PERSON_INFO_SIZE*sizeof(int), MS_SYNC);
    munmap(bd, MAX_USERS*PERSON_INFO_SIZE*sizeof(int));


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
    server.sin_port = htons((short int)66666);


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


    int arr[MAX_USERS];
    for(int i = 0 ; i < MAX_USERS ; i++)arr[i]=i+OFFSET_FOR_SOCKFD;
    int counter = 0;
    char *names[MAX_USERS] = {NULL};

    do {
        if ((msgsock = accept(sock,(struct sockaddr *) NULL,(socklen_t *) NULL)) == -1) perror("accept");
        else{
            int i;
            char ibuf[BUF];
            char* name;
            memset(ibuf, 0, BUF);
            if ((rval = read(msgsock, ibuf, 1024)) == -1)perror("reading stream message");

            if(strncmp(ibuf,"POST",4)==0){
                dprintf(1,"[%s]\n",ibuf);
                for(i = 5 ; ibuf[i] != ' ' ; i++){
                }
                name = malloc(sizeof(char)*(i-5));
                
                for(i = 5 ; ibuf[i] != ' ' ; i++){
                    name[i-5] = ibuf[i];
                }
            }
            else if(strncmp(ibuf,"GET",3)==0){
                dprintf(1,"[%s]\n",ibuf);
                for(i = 4 ; ibuf[i] != ' ' ; i++){
                }
                name = malloc(sizeof(char)*(i-4));
                
                for(i = 4 ; ibuf[i] != ' ' ; i++){
                    name[i-4] = ibuf[i];
                }
            }

            dprintf(1,"name[%s] strlen name = %ld \n",name,strlen(name));
            for(i = 0 ; i < counter ; i++){
                dprintf(1,"%s ",names[i]);
                if(strcmp(names[i],name)==0){
                    dup2(msgsock, arr[i]);
                    close(msgsock);
                    cpid = fork();
                    dprintf(1,"%d start SESSION\n",arr[i]);
                    if(cpid == 0){
                        session(arr[i],arr,counter,names,ibuf,name,fd_bd);
                    }
                    break;
                }
            }
            if(i == counter){
                dprintf(1,"accept con ");
                dup2(msgsock, arr[counter]);
                close(msgsock);
                dprintf(1,"msgsock = %d \n",arr[counter]);
                cpid = fork();
                if(cpid == 0){
                    session(arr[counter],arr,counter,names,ibuf,name,fd_bd);
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

        } 
    } while(1);

    exit(0);
}