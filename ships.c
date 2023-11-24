#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <dirent.h>

#define errExit(msg)    do { perror(msg); _exit(EXIT_FAILURE); } while (0)

#define BUF 2048
#define BUF_UNIX 100


#define START_UNIX_SOCK_PATH "/tmp/unix_start1.server"

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
};

void generate_shots(int shots[10][10]){
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0 ; j < 10 ;j++){
            shots[i][j]=0;
        }
    }
}

void print_matrix(int mat[10][10]){
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



void session(int msgsock,int arr[],int counter,char* names[],char ibuf[],char* name,struct info arr_info[]){

  dprintf(1,"%d start SESSION\n",msgsock);
    int n;   
   int sock;
   socklen_t length;
   struct sockaddr_in server;
   char obuf[BUF];
   int rval,sval;
   const int enable = 1; 


//    do {
    //   if ((rval = read(msgsock, ibuf, 1024)) == -1)
    //      perror("reading stream message");
    //   dprintf(1,"[%d]\n",rval);
    //   if (rval == 0)
    //      printf("Connection finished\n");
         
      if(strncmp(ibuf,"GET",3)==0){
        //  dprintf(1,"[%s]\n",ibuf);

         struct stat filestat;
         int f;
         int i;
         for(i = 4 ; ibuf[i] != ' ' ; i++){
         }
         char* name = malloc(sizeof(char)*(i-4));
         
         for(i = 4 ; ibuf[i] != ' ' ; i++){
            name[i-4] = ibuf[i];
         }
         dprintf(1,"[%s]\n",name);


        int fd;
        if((fd=open("/tmp/fifo001", O_RDWR)) == - 1)  {
            perror("open fifo");
            exit(0);
        }
        write(fd,name,strlen(name));

        // if(strcmp(name,"/favicon.ico")==0){
        //     exit(1);
        // }

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
      }
      else if(strncmp(ibuf,"POST",4)==0){
                //  dprintf(1,"[%s]\n",ibuf);

            struct stat filestat;
            int f;
            int i;
            // for(i = 5 ; ibuf[i] != ' ' ; i++){}
            // char* name = malloc(sizeof(char)*(i-5));
            
            // for(i = 5 ; ibuf[i] != ' ' ; i++){
            //     name[i-5] = ibuf[i];
            // }
            // dprintf(1,"name[%s]\n",name);
            dprintf(1,"%d start POST\n",msgsock);

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
                // no such user
            }
            else{
                if(strcmp(mes,"start")==0){
                    gen_field(arr_info[c].myShips,arr_info[c2].myShips_info,0);
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
                            // printf("send %d ",rc);
                        }
                        for(int j = 0 ; j < 10 ; j++){
                            if(arr_info[c].myShots[i][j]==0)rc = send(client_sock, "0" , 1 , 0);
                            else rc = send(client_sock, "1" , 1 , 0);
                            // printf("send %d ",rc);
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
                            // printf("send %d ",rc);
                        }
                        for(int j = 0 ; j < 10 ; j++){
                            if(arr_info[c2].myShots[i][j]==0)rc = send(client_sock2, "0" , 1 , 0);
                            else rc = send(client_sock2, "1" , 1 , 0);
                            // printf("send %d ",rc);
                        }
                    }
                    rc = send(client_sock2, "F" , 1 , 0);
                    rc = recv(client_sock2, obuf, BUF,0);
                    // sval=send(msgsock,obuf,strlen(obuf),0);
                   
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

               

                // if(strcmp(name,"/favicon.ico")==0){
                //     exit(1);
                // }

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
                dprintf(1,"send_to_sender[%d]",sval);
                }
                
            }


                dprintf(1,"send_to_rec[%d]",sval);
                int fd;
                if((fd=open("/tmp/fifo001", O_RDWR)) == - 1)  {
                    perror("open fifo");
                    exit(0);
                }
                write(fd,name,strlen(name));
            
      }
//    } while ((rval > 0) && (strncmp(ibuf,GET,3)));


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
    char mat_inc[14][14] = {0};
    struct info arr_info[14];
   
    int fd;
    int rd;
    char buf_fifo[80];
  unlink("/tmp/fifo001");
  if((mknod("/tmp/fifo001", S_IFIFO|0777,0)) == -1)  {
    perror("make fifo");
    exit(0);
  }
  if((fd=open("/tmp/fifo001", O_RDWR)) == - 1)  {
    perror("fifo");
    exit(0);
  }
  bzero(buf_fifo,80);


  
  if(rd == -1) perror("fifo");
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
                if(cpid == 0){
                    dprintf(1,"%d start SESSION\n",arr[i]);
                    session(arr[i],arr,counter,names,ibuf,name,arr_info);
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
                session(arr[counter],arr,counter,names,ibuf,name,arr_info);
            }
        }


        if((rd=read(fd, &buf_fifo, sizeof(buf_fifo))) > -1 && strcmp(buf_fifo,"/favicon.ico")){
            dprintf(1,"Data[%d]: ",rd);
            dprintf(1,"%s \n",buf_fifo);

            int i;
            int flag = 0;
            dprintf(1,"%d ",counter);
            for(i = 0 ; i < counter ; i++){
                dprintf(1,"%s ",names[i]);
                if(strcmp(names[i],buf_fifo)==0){flag=1;dprintf(1,"%s %s\n",names[i],buf_fifo);}
            }
            if(!flag){
                names[counter] = malloc(strlen(buf_fifo)+1);
                strcpy(names[counter],buf_fifo);
                counter++;
            }
         }
           } 
    bzero(buf_fifo,80);
   } while(strcmp(ibuf,FIN));
   
   exit(0);
}