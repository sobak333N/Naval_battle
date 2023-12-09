#define DATA1 "\
HTTP/1.1 200 OK\n\
Content-Type: text/html;charset=utf-8\n\
\n\
"

#define ERROR404 "\
HTTP/1.1 404 Not Found\n\
Content-Type: text/html;charset=utf-8\n\
\n\
<html>\n\
<head></head>\n\
<body>\n\
<a>ERROR404 No such directory</a>\n\
</body>\n\
</html>\n\
"
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
struct user_info{
    int inGame;
    int op;
    int myTurn;
    int left;
    int myShips[10][10];
    int myShots[10][10];
    int myShips_info[10][5];
};
void feel_two_xd(int field[10][10] , int j , int ships[10][5]);
void rand_pos(int ships[10][5] , int num);
void feel_one(int field[10][10], int j , int ships[10][5]);
void gen_field(int field[10][10],int ships[10][5],int t);
void tmp_user(struct user_info* user, int offset , int* bd);
void print_matrix(int mat[10][10]);
void generate_shots(int shots[10][10]);
int open_unix_sock(char* name_of_prog);

#define BUF 2048
#define BUF_UNIX 100
#define PERSON_INFO_SIZE 254
#define MAX_USERS 50
#define OFFSET_FOR_SOCKFD 11

#define START_UNIX_SOCK_PATH "/tmp/unix_start1.server"
#define HANDLER_MOVE_UNIX_SOCK_PATH1 "/tmp/unix_handler_move1.server"
#define HANDLER_MOVE_UNIX_SOCK_PATH2 "/tmp/unix_handler_move2.server"
#define ERROR_START__UNIX_SOCK_PATH1 "/tmp/unix_error1.server"
#define HANDLER_CHAT_UNIX_SOCK_PATH "/tmp/unix_chat.server"
#define CLIENT_PATH "unix_sock.client"

#define FIN "exit"
#define GET "GET"
#define BD "bd"
#define DATA "Answer\n"
#define errExit(msg)    do { perror(msg); _exit(EXIT_FAILURE); } while (0)
