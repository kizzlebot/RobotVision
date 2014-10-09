#include <stdio.h>
#include <stdlib.h>
#define N_BY_N 30

int changeByRef(char ** ch){
    int i = 0 ;
    for ( i = 10 ; i < N_BY_N-10; i++  ){
        int j = 0 ;
        for ( j = 10 ; j < N_BY_N-10; j++  ){
            ch[i][j] = ch[i][j]+1;
        }
        printf("\n");
    }

    return 0 ;
}

void printCh(char ** ch){
    int i = 0 ;
    for ( i = 0 ; i < N_BY_N; i++  ){
        int j =0 ;
        for ( j = 0 ; j < N_BY_N; j++  ){
            printf("%c", *(*(ch+i)+j));
        }
        printf("\n");
    }
}

char ** alloc(int nByn){
    char ** ch = malloc(sizeof(char *)*0x100);
    int i = 0 ;
    for ( i = 0 ; i < N_BY_N; i++ ){
        *(ch+i) = malloc(sizeof(char)*0x100);
        int j= 0 ;
        for ( j = 0 ; j < N_BY_N ; j++ ){
            *(*(ch+i)+j) = 0x41 ;
        }
    }
    return ch ;
}

int main(){
    char ** ch = alloc(N_BY_N);
    printCh(ch);
    changeByRef(ch);
    printCh(ch);
}



