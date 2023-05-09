#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readInstructions(char *fileName){
    char *arr[10];
    int aux = 0;
    FILE *fp = fopen(fileName, "r");
    
    for(int i = 0; i < 10; i++) arr[i] = (char *)malloc(100 * sizeof(char));

    while(!feof(fp)){
        fgets(arr[aux], 100, fp);
        aux++;
    }

    for(int i = 0; i < 10; i++){
        arr[i][strlen(arr[i])-1] = '\0';
        printf("%s\n", arr[i]);
    } 

    for(int i = 0; i < 10; i++) free(arr[i]);

    fclose(fp);
}

int main(void){
    readInstructions("input.txt");
	return 0;
}
