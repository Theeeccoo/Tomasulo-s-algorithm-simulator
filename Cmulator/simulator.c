#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** DEFINITIONS **/
#define MAX_SIZE 20


int numberOfLines(char *file_path){
	char	foo[MAX_SIZE];
	int	lines = 0;
	FILE 	*file_pointer;

	file_pointer = fopen(file_path, "r");
	
	
	fgets(foo, MAX_SIZE, file_pointer);
	while( !feof(file_pointer) ) {
		lines++;
		fgets(foo, MAX_SIZE, file_pointer);
	}


	fclose(file_pointer);
	return lines;
}

char** readInstructions(char *file_path){
	char	**file_inputs;
	int	number_of_lines;
	FILE	*file_pointer;

	file_pointer = fopen(file_path, "r");

	if ( file_pointer != NULL ) {
		int	i, line_size = 0;;
		number_of_lines = numberOfLines(file_path);
		file_inputs = (char **) malloc(number_of_lines * sizeof(char*));

		// Since we know the ammount of lines, can be done like this V
		for( i = 0; i < number_of_lines; i++ ) {
			file_inputs[i] = (char *) malloc(MAX_SIZE * sizeof(char));
			fgets(file_inputs[i], MAX_SIZE, file_pointer);

			line_size = strlen(file_inputs[i]);
			file_inputs[i][line_size - 1] = '\0';

			printf("%s\n", file_inputs[i]);
		
		}
	
		/* Can be done like this aswell < (needs to insert malloc)
		i = 0;
		rewind(file_pointer);
		fgets(file_inputs[i], MAX_SIZE, file_pointer);
		while( !feof(file_pointer) ) {
			line_size = strlen(file_inputs[i]);
			file_inputs[i][line_size - 1] = '\0';
			printf("%s\n", file_inputs[i]);
			fgets(file_inputs[++i], MAX_SIZE, file_pointer);		
		}
		*/
	}
	
	fclose(file_pointer);
	/*char *arr[10];
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

	fclose(fp);*/
	return file_inputs;
}

int main(void){
	char	**foo = readInstructions("input.txt");
	int	size = numberOfLines("input.txt"),
		i = 0;
	printf("\n\n\n\n");

	// Would be better to find a way to make this works, otherwise we'll need to reopen the file...
	/*
	for( int i = 0; *foo != NULL; foo++, i++){
		printf("%s\n", *foo);
	}
	*/
	for( ; i < size; i++, foo++ ) 
		printf("%s\n", *foo);

	return 0;
}
