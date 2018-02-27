#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("error\n");
        return 1;
	}
    int a = (int)atoi(argv[1]);

    if(a < 2){
        printf("no\n");
        return 1;
    }
    int i;
    for(i=2; i<=a/2;i++){
        if(a==2)
            break;
        if(a%i==0){
            printf("no\n");
            return 1;
        }
    }
    printf("yes\n");
}
