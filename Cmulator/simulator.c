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

int decoder(char* operation){
	int	resp = -1;

	if ( strcmp(operation, "LW") == 0 || strcmp(operation, "SW") == 0 )  {
		resp = LOAD;
	} else if ( strcmp(operation, "ADD") == 0 || strcmp(operation, "SUB") == 0 ) {
		resp = ADD;
	} else if ( strcmp(operation, "MULT") == 0 || strcmp(operation, "DIV") == 0 ) {
		resp = MULT;
	} else if ( strcmp(operation, "BNE") == 0 || strcmp(operation, "beq") == 0 ) {
		resp = BRANCH;
	}

	return resp;
}

int noDependencies(int qj, int qk){
	printf("(%d) %d && (%d) %d = %d\n", qj, qj == -1, qk ,qk == -1, ((qj == -1) && (qk == -1)));
	return ((qj == -1) && (qk == -1));
}


void initializer(char* filename){
	
	Reorder_Buffer *rb = reorderBufferInitializer();
	Instruction *instructions = instructionsInitializer(filename);
	Reservation_Station *rs = reservationStationInitializer();
	//int number_of_instructions = numberOfLines(filename); 
	int init = rb->filled_lines % MAX_LINES,
	    end  = ((init - 1) % MAX_LINES < 0) ? MAX_LINES - 1 : (init - 1);
	int i, j, old_position = 0;

	
	printInstructions(instructions, filename);
	
	while ( rb->line[end].instruction_state != COMMITED ) {
		// Adding into Reorder Buffer
		init = rb->filled_lines % MAX_LINES;
		end = ( (init - 1) % MAX_LINES < 0 ) ? MAX_LINES - 1 : (init - 1);
		for ( i = init ; i != end; i = (i + 1) % MAX_LINES ) {
			if ( rb->line[i].instruction_execution != BUSY ) {
				int position = insertInstructionRB( &instructions[old_position], rb );
				rb->line[i].instruction->reorder_buffer_position = position;
				rb->line[i].instruction->type = decoder( rb->line[i].instruction->splitted_instruction[0] );

				old_position ++;
			}
		}
		if ( rb->line[i].instruction_execution != BUSY ) {
			int position = insertInstructionRB( &instructions[old_position], rb );
			rb->line[i].instruction->reorder_buffer_position = position;
			rb->line[i].instruction->type = decoder( rb->line[i].instruction->splitted_instruction[0] ); 
		}


		// Adding into Reservation Station
		init = rb->filled_lines % MAX_LINES;
		end = ( (init - 1) % MAX_LINES < 0 ) ? MAX_LINES - 1 : (init - 1);

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
		
		fflush(stdin);
		printf("\n\n**Press Enter to continue your execution. . .\n");
		getchar();
		system("cls || clear");

		// Execution 
		for ( i = 0; i < MAX_LINES_RS; i++ ){
			if (noDependencies( rs->line[i].information_dependency_Qj, rs->line[i].information_dependency_Qk ) == 1) {
				int inst_position = rs->line[i].position_destination_rb;
				rb->line[inst_position].instruction_state = WRITE_RESULT;
				// TODO ARRUMAR O RESULT AQUI O >>>
				strcpy( rb->line[inst_position].instruction_result, "Terminado aqui.");
				
				for ( j = 0; j < MAX_LINES_RS; j++ ){
					if ( rs->line[j].information_dependency_Qj == inst_position ) {
						rs->line[j].value_register_read_Vj = (char*) malloc( sizeof(char) * SIZE_STR );
						strcpy( rs->line[j].value_register_read_Vj, rb->line[inst_position].instruction->splitted_instruction[1] );
						rs->line[j].information_dependency_Qj = -1;
					}
					if ( rs->line[j].information_dependency_Qk == inst_position ) {
						rs->line[j].value_register_read_Vk = (char*) malloc( sizeof(char) * SIZE_STR );
						strcpy( rs->line[j].value_register_read_Vk, rb->line[inst_position].instruction->splitted_instruction[1] );
						rs->line[j].information_dependency_Qk = -1;
					}
					printReservationStation(rs);
					fflush(stdin);
					getchar();
					system("cls || clear");
				}
				// TODO LIMPAR A LINHA EXECUTADA DA ESTACAO DE RESERVA
				//      CHECAR DESVIO
			}
			printReservationStation(rs);
			fflush(stdin);
			getchar();
			system("cls || clear");
		}
		
		// TODO COMMIT REORDER BUFFER
		//      DEPENDENCIA FALSA (Renomeacao)
		//      CHECAR DESVIO (nao sabemos se sera aqui) 


		printReorderBuffer(rb);
		printReservationStation(rs);
		
		fflush(stdin);
		printf("\n\n**Press Enter to continue your execution. . .\n");
		getchar();
		system("cls || clear");
	}
	
	



	/**
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
	*/

	
	

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
