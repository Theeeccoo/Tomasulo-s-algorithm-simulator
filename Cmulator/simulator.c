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

/**
 * @brief Decode instruction stating which functional unit it will be executed
 *
 * @param operation Operation that the instruction
 * 
 * @details Decode instruction stating which functional unit it will be executed
 * 
 * @return If the instruction type is supported by the simulator, its type will be returned, otherwise -1 will be returned.
 */
int decoder(char* operation){
	int	resp = -1;

	if ( strcmp(operation, "LW") == 0 || strcmp(operation, "SW") == 0 )  {
		resp = LOAD;
	} else if ( strcmp(operation, "ADD") == 0 || strcmp(operation, "SUB") == 0 ) {
		resp = ADD;
	} else if ( strcmp(operation, "MUL") == 0 || strcmp(operation, "DIV") == 0 || strcmp(operation, "REM") == 0 ) {
		resp = MULT;
	} else if ( strcmp(operation, "BNE") == 0 || strcmp(operation, "BEQ") == 0 ) {
		resp = BRANCH;
	} else if ( strcmp(operation, "SLT") == 0 || strcmp(operation, "SLTI") == 0 ) {
		resp = COMPARISON;
	} else if ( strcmp(operation, "OR") == 0 || strcmp(operation, "AND") == 0 || strcmp(operation, "XOR") == 0 || 
				strcmp(operation, "SLL") == 0 || strcmp(operation, "SRL") == 0 || strcmp(operation, "SRA") == 0 ) {
		resp = LOGICAL;
	}

	return resp;
}

/**
 * @brief Checks if the statement has true dependency on another statement
 *
 * @param qj First read register dependency information
 * @param qk Read second register dependency information
 * 
 * @details Checks if the statement has true dependency on another statement
 * 
 * @return If there is no dependency, 1 is returned, otherwise 0
 */
int noDependencies(int qj, int qk){
	return ((qj == -1) && (qk == -1));
}

void initializer(char* filename){
	
	Reorder_Buffer *rb = reorderBufferInitializer();
	Instruction *instructions = instructionsInitializer(filename);
	Reservation_Station *rs = reservationStationInitializer();
	Register_status *registerRename = registerStatusInitializer();
	int number_of_instructions = numberOfLines(filename); 
	int init = rb->filled_lines % MAX_LINES,
	    end  = ((init - 1) % MAX_LINES < 0) ? MAX_LINES - 1 : (init - 1);
	int i, j, old_position = 0;

	printf("Number of lines: %d\n", number_of_instructions);
	
	printInstructions(instructions, filename);
	
	while ( rb->line[end].instruction_state != COMMITED ) {
		// Adding into Reorder Buffer
		// TODO - Prever desvio e tomar decisão de desvio aqui, tomar cuidado para conseguir recuperar a instrução
		// se não houver desvio
		if (old_position < number_of_instructions) {
			init = rb->filled_lines % MAX_LINES;
			end = ( (init - 1) % MAX_LINES < 0 ) ? (MAX_LINES - 1) : (init - 1);
			for ( i = init ; i != end; i = (i + 1) % MAX_LINES ) {
				if ( rb->line[i].instruction_execution != BUSY ) {

					// Check if free position is from an instruction that has already been committed and writes in register
					if (rb->line[i].instruction_state == COMMITED && dontDoWrite(rb->line[i].instruction->splitted_instruction[0]) == 0) {
						// Release the instruction register that was previously in this line from the reorder buffer, as the line will receive another instruction
						freeRegister(rb->line[i].instruction->splitted_instruction[1], registerRename);
					}

					// Insert instruction from queue into reorder buffer
					int position = insertInstructionRB( &instructions[old_position], rb );
					rb->line[i].instruction->reorder_buffer_position = position;
					rb->line[i].instruction->type = decoder( rb->line[i].instruction->splitted_instruction[0] );

					old_position ++;
				}
			}
			if ( rb->line[i].instruction_execution != BUSY ) {
				// Check if free position is from an instruction that has already been committed and writes in register
				if (rb->line[i].instruction_state == COMMITED && dontDoWrite(rb->line[i].instruction->splitted_instruction[0]) == 0) {
					// Release the instruction register that was previously in this line from the reorder buffer, as the line will receive another instruction
					freeRegister(rb->line[i].instruction->splitted_instruction[1], registerRename);
				}
				int position = insertInstructionRB( &instructions[old_position], rb );
				rb->line[i].instruction->reorder_buffer_position = position;
				rb->line[i].instruction->type = decoder( rb->line[i].instruction->splitted_instruction[0] ); 
				old_position ++;
			}
		}

		// Adding into Reservation Station
		init = rb->filled_lines % MAX_LINES;
		end = ( (init - 1) % MAX_LINES < 0 ) ? (MAX_LINES - 1) : (init - 1);

		for ( i = init; i != end; i = (i + 1) % MAX_LINES ) {
			if ( rb->line[i].instruction_state == WAITING ) { 
				int logical = 0 ;
				if ( (logical = insertInstructionRS(rb->line[i].instruction, rs, rb)) != -1 )
					rb->line[i].instruction_state = ISSUE;
			}
		}
		if ( rb->line[i].instruction_state == WAITING ) {
			int logical = 0;
			if ( (logical = insertInstructionRS(rb->line[i].instruction, rs, rb)) != -1){
				rb->line[i].instruction_state = ISSUE;
			}		
		}

		printReorderBuffer(rb);
		printReservationStation(rs);
		//printRegisterStatus(registerRename);
		
		fflush(stdin);
		printf("\n\n**Press Enter to continue your execution. . .\n");
		getchar();
		system("cls || clear");

		// Execution 
		for ( i = 0; i < MAX_LINES_RS; i++ ){
			if ( rs->line[i].reservation_busy == NOT_BUSY ) continue; 
			if ((noDependencies( rs->line[i].information_dependency_Qj, rs->line[i].information_dependency_Qk ) == 1) && 
				rb->line[rs->line[i].position_destination_rb].instruction_state == ISSUE) {
				int inst_position = rs->line[i].position_destination_rb;
				rb->line[inst_position].instruction_state = EXECUTING;
				printReorderBuffer(rb);
				//printRegisterStatus(registerRename);
				fflush(stdin);
				getchar();
				system("cls || clear");

				// If instruction writes to register, put position of reorder buffer where instruction is in destination register
				if (dontDoWrite(rb->line[inst_position].instruction->splitted_instruction[0]) == 0) {
					insertRegisterStatus( rb->line[inst_position].instruction->splitted_instruction[1], inst_position, registerRename);
				}
				rb->line[inst_position].instruction_state = WRITE_RESULT;
				// Get the result and write it to the reorder buffer
				strcpy( rb->line[inst_position].instruction_result, calculateResult(rb->line[inst_position].instruction));
				printReorderBuffer(rb);
				//printRegisterStatus(registerRename);
				fflush(stdin);
				getchar();
				system("cls || clear");
				
				// Forward of data, because the instruction was executed and its result can be used by the next instructions
				for ( j = 0; j < MAX_LINES_RS; j++ ){
					if ( rs->line[j].reservation_busy == NOT_BUSY ) continue;
					if ( rs->line[j].information_dependency_Qj == inst_position ) {
						if (rs->line[j].value_register_read_Vj == NULL) {
							rs->line[j].value_register_read_Vj = (char*) malloc( sizeof(char) * SIZE_STR );
						}
						strcpy( rs->line[j].value_register_read_Vj, rb->line[inst_position].instruction->splitted_instruction[1] );
						rs->line[j].information_dependency_Qj = -1;
					}
					if ( rs->line[j].information_dependency_Qk == inst_position ) {
						if (rs->line[j].value_register_read_Vk == NULL) {
							rs->line[j].value_register_read_Vk = (char*) malloc( sizeof(char) * SIZE_STR );
						}
						strcpy( rs->line[j].value_register_read_Vk, rb->line[inst_position].instruction->splitted_instruction[1] );
						rs->line[j].information_dependency_Qk = -1;
					}
					printReservationStation(rs);
					fflush(stdin);
					getchar();
					system("cls || clear");
				}
				// Release reservation station that had an instruction that has already been executed
				clearLineRS(rs, i);
				// TODO - CALCULAR SE HAVERA OU NÃO O DESVIO DE INSTRUÇÃO, SE SIM VERIFICAR SE INSTRUÇÕES FORAM DESCARTADAS, SE NÃO, DESCARTAR INSTRUÇÕES
			}
			printReservationStation(rs);
			fflush(stdin);
			getchar();
			system("cls || clear");
		}
		
		// Commit the statement from the first position of the reorder buffer if it has already been executed, as the commit needs to be in order
		init = rb->filled_lines % MAX_LINES;
		end  = ((init - 1) % MAX_LINES < 0) ? (MAX_LINES - 1) : (init - 1);
		int controll_commit = 0;
		
		for ( i = init; i != end && controll_commit == 0; i = (i + 1) % MAX_LINES ) {
			if (rb->line[i].instruction_state == WRITE_RESULT) {
				rb->line[i].instruction_state = COMMITED;
				rb->line[i].instruction_execution = NOT_BUSY;
				// Control to have only 1 commit at a time
				controll_commit = 1;
			}
		}
		if (controll_commit == 0 && rb->line[end].instruction_state == WRITE_RESULT) {
			rb->line[end].instruction_state = COMMITED;
			rb->line[end].instruction_execution = NOT_BUSY;
		}
	

		printReservationStation(rs);
		printReorderBuffer(rb);
		//printRegisterStatus(registerRename);
		
		fflush(stdin);
		printf("\n\n**Press Enter to continue your execution. . .\n");
		getchar();
		system("cls || clear");
	}

	printf("ACABOOOOOOOU!\n");
}

int main(void){
	initializer("input.txt");
	return 0;
}
