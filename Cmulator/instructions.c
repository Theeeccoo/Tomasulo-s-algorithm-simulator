#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** DEFINITIONS **/
#define MAX_SIZE 20

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
	char	*delimiter = " ,()";
	char	*token;
	int	aux = 0;

	token = strtok(instruction, delimiter);
	while( token != NULL ) {
		*splitted_string = (char*) malloc( 10 * sizeof(char) );
		strcpy(*splitted_string, token); 
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

/**
 * @brief Initializing all instructions
 *
 * @param filename that contains all instructions to be read
 *
 * @details Creates a array of instructions that are read from the file of given path.
 *
 * @return Referece to a group of instructions
 */
Instruction* instructionsInitializer(char* filename){
	int	number_of_lines = 0,
		i = 0;
	number_of_lines = numberOfLines(filename);
	Instruction *instructions = (Instruction*) malloc( number_of_lines * sizeof(Instruction) );

	char	**instructionsRead = readInstructions(filename, number_of_lines);
	
	for( i = 0; i < number_of_lines; i++ ) {
		instructions[i].full_instruction = (char*) malloc ( SIZE_STR * sizeof(char) );
		strcpy(instructions[i].full_instruction, instructionsRead[i]);
		instructions[i].splitted_instruction = splitInstruction(instructionsRead[i]);
		instructions[i].time = (float*) calloc( 5, sizeof(float) );
	}	

	return instructions;
}


void printInstructions(Instruction *it, char* filePath){
	int	i,
		size = numberOfLines(filePath);
	printf("Instrucoes: \n");
	for ( i = 0; i < size ; i++ ){
		printf("%s\n", it[i].full_instruction);
	}
}

/**
 * @brief Insert time of entry into reservation station, execution or commit
 *
 * @param full_instruction 		Instruction that will have its result calculated
 * @param instructions			Structure that has all the instructions
 * @param position				Position that informs which time is being inserted 
 * 								in the instruction
 * @param amount_instructions	Number of instructions in total that will be iterated
 * @param seconds				Seconds that will be stored in the corresponding position
 * 
 * @details The position passed by parameter informs which time will be inserted, 
 * 			according to stages in the reorder buffer of the instruction
 */
void insertTime (char* full_instruction, Instruction *instructions, int position, int amount_instructions, float seconds) {

	for (int i = 0; i < amount_instructions; i++) {
		if (strcmp(instructions[i].full_instruction, full_instruction) == 0 && instructions[i].time[position] == 0) {
			instructions[i].time[position] = seconds;
			i = amount_instructions;
		}
	}
}

/**
 * @brief Prints all times of the instruction
 *
 * @param instructions			Structure that has all the instructions
 * @param amount_instructions	Number of instructions in total that will be iterated
 * 
 * @details Prints all times of the instruction referring to the entry in the reorder 
 * 			buffer, reservation station, when it was executed, when the write result 
 * 			was done and when it was committed
 */
void printTimeInstructions(Instruction *instructions, int amount_instructions) {

	printf("\nInstruction times:\n\n");
	printf("Instruction\t\tWAITING\t\tISSUE\t\tEXECUTING\tWRITE_RESULT\tCOMMITED\n");

	for (int i = 0; i < amount_instructions; i++) {
		printf("%s\t\t", instructions[i].full_instruction);
		printf("%.2fms\t\t",instructions[i].time[0]);
		printf("%.2fms\t\t",instructions[i].time[1]);
		printf("%.2fms\t\t",instructions[i].time[2]);
		printf("%.2fms\t\t",instructions[i].time[3]);
		printf("%.2fms\n",instructions[i].time[4]);
	}
}