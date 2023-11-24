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
void feel_two_xd(int field[10][10] , int j , int ships[10][5]);
void rand_pos(int ships[10][5] , int num);
void feel_one(int field[10][10], int j , int ships[10][5]);
void gen_field(int field[10][10],int ships[10][5],int t);