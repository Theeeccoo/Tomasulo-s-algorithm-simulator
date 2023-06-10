#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reorder_buffer.h"
#include "instructions.h"
#include "reservation_station.h"
#include "register_status.h"

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
char** freeCharacterMatrix(char **matrix, int size)
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


void initializer(char* filename){
	
	Reorder_Buffer *rb = reorderBufferInitializer();
	Instruction *instructions = instructionsInitializer(filename);
	Reservation_Station *rs = reservationStationInitializer();
	
	printInstructions(instructions, filename);

	// Inserindo instrução: ADD R2, R2, R3
	instructions[0].reorder_buffer_position = insertInstructionRB(&instructions[0], rb);
	// Inserindo instrução: SUB R1, R2, R3
	instructions[1].reorder_buffer_position = insertInstructionRB(&instructions[1], rb);
	// Inserindo instrução: SW R1, 0(R1)
	instructions[5].reorder_buffer_position = insertInstructionRB(&instructions[5], rb);
	// Inserindo instrução: LW R1, 0(R1)
	instructions[4].reorder_buffer_position = insertInstructionRB(&instructions[4], rb);
	// Inserindo instrução: ADD R1, R2, R3
	instructions[7].reorder_buffer_position = insertInstructionRB(&instructions[7], rb);



	instructions[0].type = ADD;
	instructions[1].type = ADD;
	instructions[5].type = LOAD;
	instructions[4].type = LOAD;
	instructions[7].type = ADD;
	

	printf("%d", instructions[4].reorder_buffer_position );

	
	printReorderBuffer(rb);
	
	insertInstructionRS(&instructions[0], rs, rb);
	//printf("Dependecy found 1: %s\n", dependencyIdentifier(instructions, instructions[0].splitted_instruction[2], instructions[0].reorder_buffer_position, rb));
	insertInstructionRS(&instructions[1], rs, rb);
	//printf("Dependency found 2: %s\n", dependencyIdentifier(instructions, instructions[1].splitted_instruction[2], instructions[1].reorder_buffer_position, rb));
	insertInstructionRS(&instructions[5], rs, rb);
	insertInstructionRS(&instructions[4], rs, rb);
	insertInstructionRS(&instructions[7], rs, rb);

	printReservationStation(rs);

	

	// Frees pointers
	/*if (freeCharacterMatrix(barz, 5) == NULL) {
		printf("Free the matrix barz!\n");
	}
	if (freeCharacterMatrix(instructions, number_of_lines) == NULL) {
		printf("Free the matrix instructions!\n");
	}
	if (freesReorderBuffer(rb) == NULL) {
		printf("Free the reorderBuffer!\n");
	}*/

	//testing register status
	/*
	Register_status  *registerStatus = registerStatusInitializer();
	printRegisterStatus(registerStatus);
	insertRegisterStatus("F0", 3, registerStatus);
	insertRegisterStatus("F6", 6, registerStatus);
	insertRegisterStatus("F8", 4, registerStatus);
	printRegisterStatus(registerStatus);
	printf("CALMA EH PARA DAR ERRO AQUI MESMO, CALMA, A GENTE TAVA TESTANDO ERRO, CALMA, TA CERTO ESSE ERRO!\n");
	insertRegisterStatus("F12", 2, registerStatus);
	insertRegisterStatus("F9", -1 , registerStatus);
	insertRegisterStatus("F", 2, registerStatus);
	freeRegister("F0", registerStatus);
	freeRegister("F6", registerStatus);
	printRegisterStatus(registerStatus);
	*/
}

int main(void){
	initializer("input.txt");
	return 0;
}
