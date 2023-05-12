#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reorder_buffer.h"

/** DEFINITIONS **/
#define MAX_SIZE 20

/**
 * @brief Freeing memory allocated to an matrix
 *
 * @param matrix to be free
 * @param size matrix size
 * 
 * @return NULL if matrix is free, matrix otherwise
 *
 * @details One should free memory using free() function to avoid memory leaks
 *
 */
char ** freeCharacterMatrix(char **matrix, int size)
{
    int i = 0;
    if (matrix == NULL)
		printf("** Error: Invalid matrix!**\n");
        return (NULL);
    if (size < 1)
    {
        printf("** Error: Invalid size parameter!**\n");
        return (matrix);
    }
    for (i = 0; i < size; i++)
        free(matrix[i]); 	/* Frees the rows from the matrix */
    free(matrix);        /* Frees the matrix */
    return (NULL);
}


  /*=====================================*/
 /*       INSTRUCTION PREPARATION       */
/*=====================================*/

/**
 * @brief Splits a instruction into pieces containing operation and operators
 *
 * @param instruction to be splitted 
 *
 * @details Receives a line of instruction and split it on blank spaces, ex: ADD R1, R2, R3 = 1 (ADD) 2(R1,) 3(R2,) 4(R3)
 *
 */

char** splitInstruction(char *instruction){
	// Setting size the max size here as 4
	// but later we need to check if there's any instruction that needs more space ADD R1, R2, R3
	char	**splitted_string = (char**) malloc(( 5 * (sizeof(char*)) ));
    char	*delimiter = " ,";
	char	*token;
	int	aux = 0;

	token = strtok(instruction, delimiter);
	while( token != NULL ) {
		*splitted_string = (char*) malloc( 10 * sizeof(char) );
		strcpy(*splitted_string, token); 
		printf("%s\t", *splitted_string);
		splitted_string++;
		aux++;
		token = strtok(NULL, delimiter);
	}	

	*splitted_string = (char*) malloc( sizeof(char) );
	strcpy(*splitted_string, "\0");
	

	splitted_string -= aux;
	return splitted_string;
}


/**
 * @brief Counts the number of lines of a file
 *
 * @param path to file
 *
 * @details Opens a new file (based on path given), iterate through all lines and count them until the end of file (EOF)
 *
 * @returns Number of lines of given file
 */
int numberOfLines(char *file_path){
	char	foo[MAX_SIZE];
	int	lines = 0;
	FILE 	*file_pointer;

	/*
	 * There is no need to check if file exists since it was tested before in "readInstructions"
	 */
	file_pointer = fopen(file_path, "r");
	
	
	fgets(foo, MAX_SIZE, file_pointer);
	while( !feof(file_pointer) ) {
		lines++;
		fgets(foo, MAX_SIZE, file_pointer);
	}

	fclose(file_pointer);
	return lines;
}

/**
 * @brief Reads RICS-V based instructions of a given file
 *
 * @param path to file
 *
 * @details Opens a new file (based on path given), iterate through all lines and save read content into a new "string".
 * It's done in a dinamic way (doesn't matter the size of the file; thanks to the function above it'll all be read).
 *
 * @returns All read lines
 */
char** readInstructions(char *file_path, int number_of_lines){
	char	**file_inputs = NULL;
	FILE	*file_pointer;

	file_pointer = fopen(file_path, "r");

	if ( file_pointer != NULL ) {
		int	i, line_size = 0;
		file_inputs = (char **) malloc(number_of_lines * sizeof(char*));

		// Since we know the ammount of lines, we'll be allocating memory and reading content at the same time
		for( i = 0; i < number_of_lines; i++ ) {
			file_inputs[i] = (char *) malloc(MAX_SIZE * sizeof(char));
			fgets(file_inputs[i], MAX_SIZE, file_pointer);

			// Removing '\n' at the end of line read by fgets
			line_size = strlen(file_inputs[i]);
			file_inputs[i][line_size - 1] = '\0';
		}


		// Creating an "empty string" as a controller (helps to know where the instructions ended).
		file_inputs[number_of_lines] = (char *) malloc(sizeof(char));
		strcpy(file_inputs[number_of_lines], "\0");
	}	
	fclose(file_pointer);
	return file_inputs;
}

void initializer(char* filename){
	int number_of_lines = 0;
	number_of_lines = numberOfLines(filename);

	char	**instructions = readInstructions(filename, number_of_lines);		
	int	aux = 0;

	Reorder_Buffer *rb = reorderBufferInitializer();

	for( ; *instructions[0] != '\0' ; instructions++ ){
		printf("%s\n", *instructions);
		aux++;
	}
	instructions -= aux;
	
	printf("\n\n");
	char	**barz = splitInstruction(instructions[0]);
	insertInstruction(barz, rb);
	barz = splitInstruction(instructions[1]);
	insertInstruction(barz, rb);

	printReorderBuffer(rb);

	// Frees pointers
	if (freeCharacterMatrix(barz, 5) == NULL) {
		printf("Free the matrix barz!\n");
	}
	if (freeCharacterMatrix(instructions, number_of_lines) == NULL) {
		printf("Free the matrix instructions!\n");
	}
	if (freesReorderBuffer(rb) == NULL) {
		printf("Free the reorderBuffer!\n");
	}
}

int main(void){
	initializer("input.txt");
	return 0;
}
