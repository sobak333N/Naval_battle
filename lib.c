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


void tmp_user(struct user_info user, int offset , int* bd){
    user.inGame = bd[offset*PERSON_INFO_SIZE];
    user.op = bd[offset*PERSON_INFO_SIZE+1];
    user.myTurn = bd[offset*PERSON_INFO_SIZE+2];
    user.left = bd[offset*PERSON_INFO_SIZE+3];
    for(int i = 0 ; i < 50 ; i++){
        user.myShips_info[i/5][i%5] = bd[offset*PERSON_INFO_SIZE+4+i];
    }
    for(int i = 0 ; i < 100 ; i++){
        user.myShots[i/10][i%10] = bd[offset*PERSON_INFO_SIZE+54+i];
    }
    for(int i = 0 ; i < 10 ; i++){
        feel_one(user.myShips,i,user.myShips_info);
    }
    printf("inGame=%d op=%d myTurn=%d left=%d\n",user.inGame,user.op,user.myTurn,user.left);
    print_matrix(user.myShips);
    print_matrix(user.myShots);
}


