#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_field(int field[10][10]){
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0 ; j < 10 ; j++){
            printf("%d ",field[i][j]);
        }
        printf("\n");
    }
    printf("-----------------------------------------\n");
}

void pretty_print_field(int field[10][10]){
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0 ; j < 10 ; j++){
            if(field[i][j]==0)printf("_");
            if(field[i][j]==1)printf("#");
        }
        printf("\n");
    }
    printf("-----------------------------------------\n");
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
        if(ships[j][0]){
            field[i][ships[j][1]] = 1;
        }
        else{
            field[ships[j][1]][i] = 1;
        }
    }
}

void gen_field(int field[10][10]){
    int ships[10][5];
    srand(time(0));
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
    print_field(field);
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0; j < 10 ;j++){
            if(field[i][j]==2)field[i][j]=0;
        }
    }
    print_field(field);
    pretty_print_field(field);
}



int main(){
    int field[10][10] = {0};
    gen_field(field);
}